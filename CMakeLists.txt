cmake_minimum_required(VERSION 3.10)
project(CaroGame)

set(CMAKE_CXX_STANDARD 11)

add_executable(caro_game 
    src/main.cpp 
    src/model/board.cpp 
    src/model/bot.cpp 
    src/model/player.cpp 
    src/model/player_manager.cpp 
    src/controller/game_controller.cpp 
    src/view/console_view.cpp 
    src/utils/utils.cpp
)
