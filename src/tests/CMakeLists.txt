set(SRC_FILES ../LonesomeAdventure.cc ../TeamAdventure.cc)

add_executable(bottomlessBagTest bottomlessBagTest.cpp ${SRC_FILES})
add_executable(sandArrangementTest sandArrangementTest.cpp ${SRC_FILES})
add_executable(crystalSelectionTest crystalSelectionTest.cpp ${SRC_FILES})


target_link_libraries( bottomlessBagTest pthread )

target_link_libraries( sandArrangementTest pthread )

target_link_libraries( crystalSelectionTest pthread )

add_executable(benchmarkBackpack benchmarkBackpack.cpp  ${SRC_FILES})
add_executable(benchmarkMax benchmarkMax.cpp  ${SRC_FILES})
add_executable(benchmarkSort benchmarkSort.cpp  ${SRC_FILES})

target_link_libraries( benchmarkBackpack pthread )
target_link_libraries( benchmarkMax pthread )
target_link_libraries( benchmarkSort pthread )
