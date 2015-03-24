set(countdown_SOURCES main.cc
    countdown.cc application.cc mainwindow.cc settingsdialog.cc colorbutton.cc soundselect.cc)
set(countdown_MOC_HEADERS
    countdown.hh application.hh mainwindow.hh settingsdialog.hh colorbutton.hh soundselect.hh)
set(countdown_HEADERS ${countdown_MOC_HEADERS})

qt5_wrap_cpp(countdown_MOC_SOURCES ${countdown_MOC_HEADERS})

SET(LANGUAGES de_DE)
SET(LANGUAGE_TS_FILES)
FOREACH(LANGUAGE ${LANGUAGES})
  SET(TS_FILE "${CMAKE_SOURCE_DIR}/i18n/countdown_${LANGUAGE}.ts")
  SET(LANGUAGE_TS_FILES ${LANGUAGE_TS_FILES} ${TS_FILE})
  SET_SOURCE_FILES_PROPERTIES(${TS_FILE} PROPERTIES OUTPUT_LOCATION
    "${CMAKE_SOURCE_DIR}/i18n")
ENDFOREACH(LANGUAGE ${LANGUAGES})
IF(LANGUAGE_TS_FILES)
  QT5_CREATE_TRANSLATION(countdown_QM_FILES ${countdown_SOURCES} ${LANGUAGE_TS_FILES})
ENDIF(LANGUAGE_TS_FILES)

qt5_add_resources(countdown_RCC_SOURCES ../resources.qrc)

add_executable(countdown WIN32 ${countdown_SOURCES} ${countdown_MOC_SOURCES}
  ${countdown_RCC_SOURCES} ${countdown_QM_FILES})
target_link_libraries(countdown ${LIBS})

install(TARGETS countdown DESTINATION bin)
