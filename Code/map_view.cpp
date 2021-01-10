/**
 * map_view.cpp
 * Implementation of MapView class.
 */

#include "map_view.hpp"
#include <iostream>
#include <random>
#include "save_state.hpp"

namespace zpr {

    /**
     * Parametrized constructor of MapView class.
     * @param data - Struct containing data of current application. (eg. window, assets).
     * @param grid_size - Size of grid chosen by user.
     */
	MapView::MapView(SimulatorDataRef data, int grid_size): data_(data), gridSize_(grid_size){
		init();
	}
    
    /**
     * Parametrized constructor of MapView class.
     * @param map_view - MapView object to be copied.
     */
    MapView::MapView(const MapView& map_view): data_(map_view.data_), gridSize_(map_view.gridSize_), cells_(map_view.cells_){
        init();
    }

    /**
     * Method which initializes all elements in the current state to display it properly.
     */
	void MapView::init() {
        this->clicked_ = false;
        this->loadAssets();
		this->setupSelectedCellRect();
		this->backgroundTexture_.setTexture(this->data_->assets_.getTexture("Background"));
		this->backgroundTexture_.setOrigin(sf::Vector2f(800, 800));
		this->backgroundTexture_.setPosition(this->mapView_.getCenter());
		this->cellSize_ = (SCREEN_HEIGHT / this->gridSize_);
		this->row_ = -1;
		this->col_ = -1;
        this->enterGridWidth_ = this->gridSize_;
        this->enterGridHeight_ = 2;
		this->selectedCellRect_.setTexture(&this->data_->assets_.getTexture("Selected Cell"));
		this->mapView_ = sf::View(sf::FloatRect(0.f, 0.f, (float)(SCREEN_HEIGHT), (float)(SCREEN_HEIGHT)));
		this->mapView_.setViewport(calculateViewPort());
		this->generateGridLines();
        this->generateEnterBoard();
        this->mapView_.zoom(1.4f);
        this->initializeCameras();
        this->fillEnterCells();
	}

    /**
     * Method initializing all cameras
     */
    void MapView::initializeCameras(){
            sf::RectangleShape temp;
            for(int i = 0; i < 2; i++){
                camerasT_[i] = temp;
            }
    }

    /**
     * Method returning current row and col.
     * @return - Returns current row and col.
     */
	sf::Vector2i MapView::getRowCol()
	{
		return sf::Vector2i(this->row_, this->col_);
	}

    /**
     * Method checking if clicked position is inside map_view.
     * @param mouse_position - Current mouse position.
     * @return - Returns true if position is inside map_view, false otherwise.
     */
	bool MapView::isClicked(sf::Vector2i mouse_position)
	{
		if (mapView_.getViewport().contains(static_cast<float>(mouse_position.x) / SCREEN_WIDTH, static_cast<float>(mouse_position.y) / SCREEN_HEIGHT))
        {
			clicked_ = true;
			return true;
		}
		else
			return false;
	}
    /**
     * Method returning map grid size.
     * @return - Size of map's grid.
     */
	int MapView::getGridSize()
	{
		return this->gridSize_;
	}

    /**
     * Method returning size of one cell.
     * @return - Size of one cell.
     */
	int MapView::getCellSize()
	{
		return this->cellSize_;
	}

    /**
     * Method responsible for loading assets_ needed in this view.
     */
	void MapView::loadAssets()
	{
		this->data_->assets_.loadTexture("Selected Cell", SELECTED_CELL_TEXTURE);
		this->data_->assets_.loadTexture("Background", BACKGROUND_TEXTURE_FILEPATH);
		this->data_->assets_.loadTexture("Road", STREET_TEXTURE);
		this->data_->assets_.loadTexture("Turn", TURN_TEXTURE);
		this->data_->assets_.loadTexture("T_Intersection", T_INTERSECTION_TEXTURE);
		this->data_->assets_.loadTexture("Intersection", INTERSECTION_TEXTURE);
		this->data_->assets_.loadTexture("Crossing", CROSSING_TEXTURE);
        this->data_->assets_.loadTexture("Entry", ENTRY_TEXTURE);
        this->data_->assets_.loadTexture("Camera", CAMERA_TEXTURE);
	}

    /**
     * Method responsible for setting parameters of selected cell.
     */
	void MapView::setupSelectedCellRect()
	{
		this->selectedCellRect_.setSize(sf::Vector2f(SCREEN_HEIGHT / this->gridSize_, SCREEN_HEIGHT / this->gridSize_));
		this->selectedCellRect_.setTexture(&this->data_->assets_.getTexture("Selected Cell"));
	}
    
    /**
     * Method responsible for calculating viewport of this view.
     * @return - Calculated viewport.
     */
	sf::FloatRect MapView::calculateViewPort()
	{
        float rect_width = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
		float rect_left = (1.f - rect_width)/2;
		return sf::FloatRect(rect_left, 0.f, rect_width, 1.f);
	}

    /**
     * Method responsible for generating grid containing enter road.
     */
    void MapView::generateEnterBoard()
    {
        std::vector<Cell> enterCells;
        for (int i = 0; i < this->enterGridHeight_ * this->gridSize_; i++)
        {
            enterCells_.push_back(Cell(((i / gridSize_)-2), i % gridSize_));
        }
        this->enterGrid_ = std::make_unique<Grid>(enterCells_, gridSize_, enterGridHeight_);
        this->enterCells_ = enterGrid_->cells_;
    }
	
    /**
     * Method responsible for drawing grid on the map.
     */
	void MapView::drawGrid() {
        if(!isSimulating_){
            for (sf::RectangleShape line : gridLines_) {
                this->data_->window_.draw(line);
            }
        }
	}
    
    /**
     * Method responsible for drawing grid containing enter road on the map.
     */
    void MapView::drawEnterGrid() {
            for (sf::RectangleShape line : enterGridLines_) {
                this->data_->window_.draw(line);
            }
    }

    /**
     * Method responsible for drawing roads made by user.
     */
	void MapView::drawRoads(){
		for (sf::RectangleShape road : this->roads_) {
			this->data_->window_.draw(road);
		}
	}

    /**
     * Method responsible for drawing entry roads.
     */
    void MapView::drawEntryRoads(){
        for (sf::RectangleShape road : this->entryRoad_) {
            this->data_->window_.draw(road);
        }
    }

    /**
     * Method responsible for drawing vehicles.
     */
	void MapView::drawVehicles()
	{
		for (std::shared_ptr<Vehicle> vehicle : this->vehicles_) {
			this->data_->window_.draw(*vehicle);
		}
	}

    /**
     * Method responsible for drawing cameras.
     */
    void MapView::drawCameras(){
        for (sf::RectangleShape camera : this->camerasT_) {
            this->data_->window_.draw(camera);
        }
    }
    
    /**
     * Method responsible for generating grid lines representing map.
     */
	void MapView::generateGridLines() {
		int draw_prefix = this->calculatePrefix();
		for (int i = 0; i <= gridSize_; i++)
		{
			sf::RectangleShape vertical_line(sf::Vector2f(2, (gridSize_)*this->cellSize_));
			vertical_line.setPosition(sf::Vector2f(i * this->cellSize_ + draw_prefix, draw_prefix));
			sf::RectangleShape horizontal_line(sf::Vector2f((gridSize_)*this->cellSize_, 2));
			horizontal_line.setPosition(sf::Vector2f(draw_prefix, i * this->cellSize_ + draw_prefix));
			this->gridLines_.push_back(vertical_line);
			this->gridLines_.push_back(horizontal_line);
		}
	}

    /**
     * Method responsible for generating enter grid lines representing map.
     */
    void MapView::generateEnterGridLines() {
        int draw_prefix = this->calculatePrefix();
        for (int i = 0; i <= enterGridWidth_; i++){
            
            sf::RectangleShape vertical_line(sf::Vector2f(2, (enterGridHeight_)*this->cellSize_));
            vertical_line.setPosition(sf::Vector2f(i * this->cellSize_ + draw_prefix, draw_prefix- 2*this->cellSize_));
            this->enterGridLines_.push_back(vertical_line);
        }
        for (int i = 1; i <= enterGridHeight_; i++){
            
            sf::RectangleShape horizontal_line(sf::Vector2f((enterGridWidth_)*this->cellSize_, 2));
            horizontal_line.setPosition(sf::Vector2f(draw_prefix, -i * this->cellSize_ + draw_prefix));
            this->enterGridLines_.push_back(horizontal_line);
        }
    }

    /**
     * Method responsible for calculating prefix - to drawing grid in the center of view.
     * @return - Calculated prefix.
     */
	int MapView::calculatePrefix() {
		double cell_size_with_point = (double)SCREEN_HEIGHT / gridSize_;
        double the_rest = cell_size_with_point - this->cellSize_;
		int draw_prefix = the_rest * gridSize_ / 2;
		return draw_prefix;
	}

    /**
     * Method responsible for filling cells in corect way - adding or deleting road to cells.
     */
	void MapView::fillCells()
	{
		for (Cell& cell : this->cells_) {
			int row = cell.getPosition().x;
			int col = cell.getPosition().y;
			if (cell.containsRoad_ && !cell.roadDrawn_) {
				cell.roadDrawn_ = true;
				this->addUserRoad(sf::Vector2i(row, col));
			}
            if (cell.toDelete_) {
                this->deleteRoad(this->transformRowColToPixels(sf::Vector2i(row, col)));
				cell.toDelete_ = false;
            }

		}
	}

    /**
     * Method responsible for filling entry cells in corect way - adding or deleting road to entry cells.
     */
    void MapView::fillEnterCells()
    {
        for (Cell& cell : this->enterCells_) {
            int row = cell.getPosition().x;
            int col = cell.getPosition().y;
            if(row == -2 && col != 0 && col != gridSize_-1){
                this->addEnterRoad(sf::Vector2i(col, row));
            }
            else if(row == -1 && col == 4){
                this->addEnterRoad(sf::Vector2i(col, row));
            }
		}
        this->addGarage(sf::Vector2i(0, -2));
        this->addGarage(sf::Vector2i(gridSize_-1, -2));
	}

    /**
     * Method responsible for adding roads.
     * @param texture_name - Name of texture which should be added to road.
     * @param position - Position of road in row and column.
     */
    void MapView::addRoad(std::string texture_name, sf::Vector2i position){
        sf::RectangleShape road;
        road.setSize(sf::Vector2f(SCREEN_HEIGHT / this->gridSize_, SCREEN_HEIGHT / this->gridSize_));
        road.setTexture(&this->data_->assets_.getTexture(texture_name));
        road.setOrigin(sf::Vector2f(road.getSize().x / 2, road.getSize().y / 2));
        sf::Vector2f centered_position_in_pixels = this->transformRowColToPixels(sf::Vector2i(position.x, position.y));
        centered_position_in_pixels.x = centered_position_in_pixels.x + this->cellSize_ / 2 ;
        centered_position_in_pixels.y = centered_position_in_pixels.y + this->cellSize_ / 2 ;
        road.setPosition(centered_position_in_pixels);
        this->roads_.push_back(road);
    }

    /**
     * Method responsible for adding cameras.
     * @param position - Position of camera in row and column.
     */
    void MapView::addCamera(sf::Vector2i position){
        if (this->checkCameraExists(this->transformRowColToPixels(sf::Vector2i(position.x, position.y)))) { return; }
        
        sf::RectangleShape camera;
        camera.setSize(sf::Vector2f(SCREEN_HEIGHT / this->gridSize_, SCREEN_HEIGHT / this->gridSize_));
        camera.setTexture(&this->data_->assets_.getTexture("Camera"));
        camera.setOrigin(sf::Vector2f(camera.getSize().x / 2, camera.getSize().y / 2));
        sf::Vector2f centered_position_in_pixels = this->transformRowColToPixels(sf::Vector2i(position.x, position.y));
        centered_position_in_pixels.x = centered_position_in_pixels.x + this->cellSize_ / 2 ;
        centered_position_in_pixels.y = centered_position_in_pixels.y + this->cellSize_ / 2 ;
        camera.setPosition(centered_position_in_pixels);
        this->camerasT_[whichCamera_-1] = camera;
    }
    
    /**
     * Method responsible for adding entry roads.
     * @param position - Position of road in row and column.
     */
    void MapView::addEnterRoad(sf::Vector2i position){
        this->addRoad("Road", position);
        this->checkRoadsTexture();
    }

    /**
     * Method responsible for adding garages (two entry points).
     * @param position - Position of garage in row and column.
     */
    void MapView::addGarage(sf::Vector2i position){
        this->addRoad("Entry", position);
    }

    
    /**
     * Method responsible for adding user roads.
     * @param position - Position of road in row and column.
     */
    void MapView::addUserRoad(sf::Vector2i position)
    {
        if (this->checkRoadExists(this->transformRowColToPixels(sf::Vector2i(position.x, position.y)))) { return; }
        this->addRoad("Road", position);
        this->checkRoadsTexture();
    }

    /**
     * Method responsible for checking wheter roads are set with correct textures in order to make drawn road look correct and properly.
     */
	void MapView::checkRoadsTexture() {
		for (sf::RectangleShape& road : this->roads_) {
			std::shared_ptr<sf::RectangleShape> north = nullptr;
			std::shared_ptr<sf::RectangleShape> south = nullptr;
			std::shared_ptr<sf::RectangleShape> east = nullptr;
			std::shared_ptr<sf::RectangleShape> west = nullptr;
			std::vector<std::shared_ptr<sf::RectangleShape>> neighbouring_roads_ptr;
            int row = this->transformPixelsToRowCol(road.getPosition().y);
            int col = this->transformPixelsToRowCol(road.getPosition().x);
			for (sf::RectangleShape& neighbouring_road : this->roads_) {
                int neighbouring_row = this->transformPixelsToRowCol(neighbouring_road.getPosition().y);
                int neighbouring_col = this->transformPixelsToRowCol(neighbouring_road.getPosition().x);
				if (row == neighbouring_row) {
					if (col + 1 == neighbouring_col) {
						east = std::make_shared<sf::RectangleShape>(neighbouring_road);
						neighbouring_roads_ptr.push_back(east);
					}
					if (col - 1 == neighbouring_col) {
						west = std::make_shared<sf::RectangleShape>(neighbouring_road);
						neighbouring_roads_ptr.push_back(west);
					}
				}
				if (col == neighbouring_col) {
					if (row + 1 == neighbouring_row) {
						south = std::make_shared<sf::RectangleShape>(neighbouring_road);
						neighbouring_roads_ptr.push_back(south);
					}
					if (row - 1 == neighbouring_row) {
						north = std::make_shared<sf::RectangleShape>(neighbouring_road);
						neighbouring_roads_ptr.push_back(north);
					}
				}
			}
			switch (neighbouring_roads_ptr.size()) {
            case 0: road.setTexture(&this->data_->assets_.getTexture("Road")); break;
			case 1: this->choseRoadWithOneNeighbour(road, north, south, east, west, row, col); break;
			case 2: this->choseRoadWithTwoNeighbours(road, north, south, east, west); break;
			case 3: this->choseRoadWithThreeNeighbours(road, north, south, east, west); break;
			case 4: road.setTexture(&this->data_->assets_.getTexture("Intersection")); break;
			}
		}
	}

    /**
     * Method responsible for setting correct texture for road in case when road has only one connected road.
     * @param road - Road whose texture will be set.
     * @param north - RectangleShape object representing road above our chosen road.
     * @param south - RectangleShape object representing road under our chosen road.
     * @param east - RectangleShape object representing road next to our chosen road (right cell).
     * @param west - RectangleShape object representing road next to our chosen road (left cell).
     * @param row - Current row.
     * @param col - Current column.
     */
	void MapView::choseRoadWithOneNeighbour(sf::RectangleShape& road, std::shared_ptr<sf::RectangleShape> north, std::shared_ptr<sf::RectangleShape> south, std::shared_ptr<sf::RectangleShape> east, std::shared_ptr<sf::RectangleShape> west, int row, int col){
        
        if (row != -2 && (col != 0 || col != gridSize_-1))
            road.setTexture(&this->data_->assets_.getTexture("Road"));
        
		road.setRotation(0);
		if (north)
			road.setRotation(90.f);
		else if (south)
			road.setRotation(90.f);
	}

    /**
     * Method responsible for setting correct texture for road in case when road has two connected roads.
     * @param road - Road whose texture will be set.
     * @param north - RectangleShape object representing road above our chosen road.
     * @param south - RectangleShape object representing road under our chosen road.
     * @param east - RectangleShape object representing road next to our chosen road (right cell).
     * @param west - RectangleShape object representing road next to our chosen road (left cell).
     */
	void MapView::choseRoadWithTwoNeighbours(sf::RectangleShape& road, std::shared_ptr<sf::RectangleShape> north, std::shared_ptr<sf::RectangleShape> south, std::shared_ptr<sf::RectangleShape> east, std::shared_ptr<sf::RectangleShape> west)
	{
		road.setTexture(&this->data_->assets_.getTexture("Turn"));
		
		if (north && east)
            road.setRotation(0);
        
		else if (north && west)
			road.setRotation(270.f);
		
		else if (south && east)
			road.setRotation(90.f);
		
		else if (south && west)
			road.setRotation(180.f);
		
		else if (north && south){
			road.setTexture(&this->data_->assets_.getTexture("Road"));
			road.setRotation(90.f);
		}
		else if (east && west) {
			road.setTexture(&this->data_->assets_.getTexture("Road"));
			road.setRotation(0);
		}
	}

/**
 * Method responsible for setting correct texture for road in case when road has three connected roads.
 * @param road - Road whose texture will be set.
 * @param north - RectangleShape object representing road above our chosen road.
 * @param south - RectangleShape object representing road under our chosen road.
 * @param east - RectangleShape object representing road next to our chosen road (right cell).
 * @param west - RectangleShape object representing road next to our chosen road (left cell).
 */
	/*Ustawia drodze odpowiedni� tekstur� w przypadku w kt�rym droga ma trzy s�siaduj�ce drogi*/
	void MapView::choseRoadWithThreeNeighbours(sf::RectangleShape& road, std::shared_ptr<sf::RectangleShape> north, std::shared_ptr<sf::RectangleShape> south, std::shared_ptr<sf::RectangleShape> east, std::shared_ptr<sf::RectangleShape> west)
	{
		road.setTexture(&this->data_->assets_.getTexture("T_Intersection"));
		if (north && south && east)
            road.setRotation(0.f);
		else if (north && south && west)
            road.setRotation(180.f);
		
		else if (north && east && west)
			road.setRotation(270.f);
		
		else if (south && east && west)
			road.setRotation(90.f);
	}

    /**
     * Method responsible for deleting roads from the map.
     * @param position - Position of the road in row and column.
     */
    void MapView::deleteRoad(sf::Vector2f position)
    {
		int i = 0;
		if (this->checkRoadExists(this->transformRowColToPixels(sf::Vector2i(position.x, position.y)))) { return; }
        
		for (sf::RectangleShape road : roads_) {
			if (road.getPosition().x - this->cellSize_ / 2 == position.x && road.getPosition().y - this->cellSize_ / 2 == position.y){
                
				roads_.erase(roads_.begin() + i);
				road.setTexture(NULL);
			}
			i++;
		}
		this->checkRoadsTexture();
	}

    /**
     * Inherited method responsible for handling actions when camera is deleting.
     * @param which_camera - Number of camera to delete.
     */
    void MapView::updateIsDeletingCamera(int which_camera){
        this->whichCamera_ = which_camera;
        
        this->deleteCamera(sf::Vector2f(camerasT_[which_camera-1].getPosition().x, camerasT_[which_camera-1].getPosition().y));
        
        this->isAddingCamera_ = false;
    }

    /**
     * Method responsible for deleting cameras from the map.
     * @param position - Position of camera to delete.
     */
    void MapView::deleteCamera(sf::Vector2f position){
        int i = 0;
        sf::RectangleShape temp;
        
        if (this->checkCameraExists(this->transformRowColToPixels(sf::Vector2i(position.x, position.y)))) { return; }
        for (sf::RectangleShape camera : camerasT_) {
            if (camera.getPosition().x == position.x && camera.getPosition().y == position.y){
                camerasT_[whichCamera_-1] = temp;
                camera.setTexture(NULL);
            }
            i++;
        }
    }

    /**
     * Method responsible for checking if road exists on given position.
     * @param position - Position where road can exist.
     * @return - True when road exists, false otherwise.
     */
	bool MapView::checkRoadExists(sf::Vector2f position) {
		for (sf::RectangleShape road : this->roads_) {
			if (road.getPosition().x - this->cellSize_/2 == position.x && road.getPosition().y - this->cellSize_/2 == position.y) {return true;}
		}
		return false;
	}
    

    /**
     * Method responsible for checking if camera exists on given position.
     * @param position - Position where road can exist.
     * @return - True when camera exists, false otherwise.
     */
    bool MapView::checkCameraExists(sf::Vector2f position) {
        for (sf::RectangleShape camera : this->camerasT_) {
            if (camera.getPosition().x -this->cellSize_/2 == position.x && camera.getPosition().y - this->cellSize_/2 == position.y) {return true;}
        }
        return false;
    }

    /**
     * Method responsible for converting pixels to rows or columns.
     * @param pixels - Value in pixels to convert.
     * @return - Converted value in rows or columns.
     */
    int MapView::transformPixelsToRowCol(double pixels){
        int result;
        
        if(pixels < 0)
            result = floor(pixels / this->cellSize_);
        else
            result = pixels / this->cellSize_;
        
        return result;
    }

    /**
     * Method responsible for converting coordinates from rows or columns to pixels.
     * @param rowcol - Row or column to convert.
     * @return - Converted value in pixels.
     */
	sf::Vector2f MapView::transformRowColToPixels(sf::Vector2i rowcol)
	{
		float x = rowcol.x * this->cellSize_ + this->calculatePrefix();
		float y = rowcol.y * this->cellSize_ + this->calculatePrefix();
		return sf::Vector2f(x, y);
	}

    
    /**
     * Method which draws elements of map on the screen.
     */
	void MapView::draw()
	{
		this->data_->window_.setView(this->mapView_);
		this->data_->window_.draw(this->backgroundTexture_);
		this->fillCells();
        this->drawRoads();
        this->drawGrid();
        this->drawVehicles();
        this->drawCameras();
	}

    /**
     * Method which handles zooming.
     * @param zoom - Value of zoom.
     */
    void MapView::zoomViewAt(float zoom)
    {
        if (zoom < 0)
            this->mapView_.zoom(1.f/abs(zoom));
        else
            this->mapView_.zoom(zoom);
    }

    /**
     * Method which handles moving view with keys.
     * @param key - Which key was clicked.
     */
    void MapView::move(keysEnum key){
        switch (key){
        case LEFT: this->mapView_.move(-90.f, 0);
            break;
        case RIGHT: this->mapView_.move(90.f, 0);
            break;
        case UP: this->mapView_.move(0, -90.f);
            break;
        case DOWN: this->mapView_.move(0, 90.f);
            break;
        }
    }

    /**
     * Method responsible for updating coordinates of currently clicked cell.
     * @param coords - Current coordinates.
     */
	void MapView::updateSelectedCell(sf::Vector2i coords)
	{
		this->row_ = coords.x;
		this->col_ = coords.y;
	}

    /**
     * Method responsible for update vector of cells on the map.
     * @param cells - Vector of new cells.
     */
	void MapView::updateCells(std::vector<Cell> cells)
	{
        this->cells_ = cells;
	}

    /**
     * Method responsible for update vector of roads on the map.
     * @param roads - Vector of new roads.
     */
    void MapView::updateRoads(std::vector<sf::RectangleShape> roads)
    {
        this->roads_ = roads;
    }

    /**
     * Method responsible for updating variable which tells MapView whether we are in drawing road mode.
     * @param is_drawing_road - True if we are drawing road, false otherwise.
     */
	void MapView::updateIsDrawingRoad(bool is_drawing_road)
	{
		this->isDrawingRoad_ = is_drawing_road;
        this->isDeletingRoad_ = false;
        this->isSimulating_ = false;
	}

    /**
     * Method responsible for updating variable which tells MapView whether we are in deleting road mode.
     * @param is_deleting_road - True if we are deleting road, false otherwise.
     */
    void MapView::updateIsDeletingRoad(bool is_deleting_road)
    {
        this->isDeletingRoad_ = is_deleting_road;
        this->isDrawingRoad_ = false;
        this->isSimulating_ = false;
    }

    /**
     * Method responsible for updating variable which tells MapView whether we are in simulating mode.
     * @param is_simulating - True if we are simulating, false otherwise.
     */
    void MapView::updateIsSimulating(bool is_simulating){
        this->isSimulating_ = is_simulating;
        this->isDrawingRoad_ = false;
        this->isDeletingRoad_ = false;
    }

    /**
     * Method responsible for updating variable which tells MapView whether we are in adding camera mode.
     * @param is_adding_camera - True if we are simulating, false otherwise.
     * @param which_camera - Number of which camera we are adding.
     * @param row - Row where should camera be placed.
     * @param col - Column where should camera be placed.
     */
    void MapView::updateIsAddingCamera(bool is_adding_camera, int which_camera, int row, int col){
        this->isAddingCamera_ = is_adding_camera;
        this->isDrawingRoad_ = false;
        this->isDeletingRoad_ = false;
        this->whichCamera_ = which_camera;
        for (Cell& cell : this->cells_) {
            int row = cell.getPosition().x;
            int col = cell.getPosition().y;
            if (cell.containsCamera_ && !cell.cameraDrawn_) {
                cell.cameraDrawn_ = true;
                this->addCamera((sf::Vector2i(row, col)));
            }
        }
    }

    /**
     * Method responsible for creating save state where we can save our map.
     */
    void MapView::saveToFile(){
        this->data_->machine_.addState(StateRef(new SaveState(this->data_, this->cells_, this->gridSize_)), false);
    }

    /**
     * Method responsible for updating vector of vehicles that are on the map.
     * @param vehicles - Vector of the vehicles.
     */
	void MapView::updateVehicles(std::vector<std::shared_ptr<Vehicle>> vehicles)
	{
		this->vehicles_ = vehicles;
	}

    /**
     * Method returning cells which are on the map.
     * @return - Vector of current cells.
     */
    std::vector<Cell> MapView::getCells(){
        return cells_;
    }
    
    /**
     * Method responsible for initial handling user clicks on the map.
     * @param mouse_position - Position of the mouse.
     * @return - Possible selected row and column.
     */
	sf::Vector2i MapView::handleInput(sf::Vector2f mouse_position)
	{
		int possible_sellected_row = (int)(mouse_position.y / this->cellSize_);
		int possible_sellected_col = (int)(mouse_position.x / this->cellSize_);
		return sf::Vector2i(possible_sellected_row, possible_sellected_col);
	}

    /**
     * Method returning View object representing map.
     * @return - sf::View mapView this mapView object.
     */
	sf::View MapView::getView()
	{
		return this->mapView_;
	}

    /**
     * Overloaded << operator, which is responsible for printing mapView object.
     * @param os - Ofstream object.
     * @param current_map_view - Current mapView object which is to print.
     * @return - Ofstream to where data is passed.
     */
    std::ofstream& operator<< (std::ofstream& os, const MapView& current_map_view){
        os << current_map_view.gridSize_ << std::endl;
        for(auto p : current_map_view.cells_)
            os << p.getPosition().x <<" & "<< p.getPosition().y<<" % "<<p.containsRoad_<<std::endl;
        return os;
    }
}
