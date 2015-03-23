set(timer_SOURCES main.cc
    main.cc countdown.cc application.cc mainwindow.cc)
set(timer_MOC_HEADERS
    countdown.hh application.hh mainwindow.hh)
set(timer_HEADERS ${plot232_MOC_HEADERS})

qt5_wrap_cpp(timer_MOC_SOURCES ${timer_MOC_HEADERS})

add_executable(timer ${timer_SOURCES} ${timer_MOC_SOURCES})
target_link_libraries(timer ${LIBS})

install(TARGETS timer DESTINATION bin)
