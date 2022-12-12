# Safest Path

## Description
Safest Path is a program that determines the route with minimum crash risk between two points in a given road network. The program is currently set up to take the public road network of Illinois using publicly available road, traffic, and crash data from the Illinois Department of Transportation.

## Installation
Download these files from [here](https://drive.google.com/drive/folders/1WS6gePORfkNFFUgHZT6UJieiOhNokvcF?usp=share_link ) and place them into the `data` folder (these files are too large for GitHub to host for free). Alternatively, place the files where you please and note their addresses.

## Usage
### Initalization and setup
To compile the program, enter the command `make main` in your terminal. 

### Input
If the datasets are in the `data` folder, enter `./main.out` into your terminal. If not, enter `./main.out road_data_address traffic_data_address crash_data_address`. Enter `y` or `n` to choose to enter road names or signed GPS coordinates, respectively. Then enter road names in the format `"road_name_1" "road_name_2"` or signed GPS coordinates `longitude_1 latitude_1 longitude_2 latitude_2` depending on your y/n choice. Finish by entering where you want to output the PNG of the safest path. Make sure to end the address with ".png".

### Viewing
To see your results, simply go to where you generated the image and open it. The image should have the safest route marked in red on top of the road network of Illinois.

### Testing
To run the test suite, you must place the datasets into the `data` folder. Once done, compile by typing `make tests`, then run by typing `./tests.out`. Tests are split into three categories, ReaderTests, ImageTests, and GraphTests. ReaderTests test the Reader and see if it can successfully read and store data from the datasets. ImageTests test Image construction and pixel manipulation. These tests are programmatically confirmed but can be visually confirmed, as it outputs PPM images into the `tests` folder. Note that the Windows OS does not have a default PPM reader, so one must be downloaded for Windows users. GraphTests test RoadGraph construction (i.e., the empty constructor along with insertNode and insertEdge), Dijkstra's algorithm, BFS, and findNearestNeighbor.

## Authors and acknowledgment
Anuj Patel, Akul Patel, David Yu, Ryan Hertzberg
