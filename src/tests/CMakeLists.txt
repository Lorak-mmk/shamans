add_executable(bottomlessBagTest bottomlessBagTest.cpp ../LonesomeAdventure.cc ../TeamAdventure.cc)
add_executable(sandArrangementTest sandArrangementTest.cpp ../LonesomeAdventure.cc ../TeamAdventure.cc)
add_executable(crystalSelectionTest crystalSelectionTest.cpp ../LonesomeAdventure.cc ../TeamAdventure.cc)


target_link_libraries( bottomlessBagTest pthread )

target_link_libraries( sandArrangementTest pthread )

target_link_libraries( crystalSelectionTest pthread )

