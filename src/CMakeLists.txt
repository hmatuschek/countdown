set(countdown_SOURCES main.cc
    countdown.cc application.cc mainwindow.cc settingsdialog.cc colorbutton.cc soundselect.cc)
set(countdown_MOC_HEADERS
    countdown.hh application.hh mainwindow.hh settingsdialog.hh colorbutton.hh soundselect.hh)
set(countdown_HEADERS ${countdown_MOC_HEADERS})

qt5_wrap_cpp(countdown_MOC_SOURCES ${countdown_MOC_HEADERS})
qt5_add_resources(countdown_RCC_SOURCES ../resources.qrc)

add_executable(countdown WIN32 ${countdown_SOURCES} ${countdown_MOC_SOURCES} ${countdown_RCC_SOURCES})
target_link_libraries(countdown ${LIBS})

install(TARGETS countdown DESTINATION bin)
