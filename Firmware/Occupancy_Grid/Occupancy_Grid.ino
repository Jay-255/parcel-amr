
int grid[30][30]; // This set's the total grid size counted in cells, where one cell is 10cm large on the x and y axis

void setup () {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Map Loading...");
  clearGrid();
  inflateObstacle(5, 5);
  inflateObstacle(16, 6);
  inflateObstacle(29, 2);
  printGrid();
}


void loop () {

}


void clearGrid(){
  // This Function clears the Grid in the memory
  for( int y = 0; y < 30;y++){
    for( int x = 0; x < 30;x++){
      grid[x][y] = 0;
    }
  }
}

void printGrid(){
  //This function Prints the Current warehouse grid in the AMR's memory
for( int y = 0; y < 30;y++){
  for( int x = 0; x < 30;x++){
    if (grid[x][y] == 0){
      Serial.print(". ");
    }
    if (grid[x][y] == 1){
      Serial.print("X ");
    }
  }
    Serial.println(" ");
}
}

void inflateObstacle(int obs_x , int obs_y ){
  //This function is used to add obstacles and specify their dimensions 
  for (int y = obs_y - 3; y <= obs_y + 3; y++){
    for (int x = obs_x - 3; x <= obs_x + 3; x++){
      if (x >= 0 && x < 30 && y >= 0 && y < 30){
      grid[x][y] = 1;
      }
    }
  }
}