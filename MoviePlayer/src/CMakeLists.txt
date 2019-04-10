SET(foo_MOC_HDRS
	Console
	FileStatusWidget.h
	graphwidget.h
	viewingWindowQt
	QCustomPlot/qcustomplot.h
)

SET(foo_RCCS jovian.qrc)
QT4_ADD_RESOURCES(foo_RCC_SRCS ${foo_RCCS})

# After this call, foo_MOC_SRCS = moc_Class1.cxx moc_Class2.cxx moc_Class3.cxx.
IF ( Qt5Widgets_FOUND )
    QT5_WRAP_CPP( foo_MOC_SRCS ${foo_MOC_HDRS} OPTIONS "-f" )
ELSE()
    QT4_WRAP_CPP( foo_MOC_SRCS ${foo_MOC_HDRS} OPTIONS "-f" )
ENDIF()

SET(TARGET_SRC 
    Communicator.cpp
    Config_Memento.cpp
    Console.cpp
    FileStatusWidget.cpp
    MyGraphicsWindowQt.cpp
    MotionCallback.cpp
    edge.cpp
    graphwidget.cpp
    hud.cpp
    node.cpp
    scene_model.cpp
    viewingWindowQt.cpp
    window_capture_callback.cpp
    QCustomPlot/qcustomplot.cpp
    Globals.cpp
    ${foo_MOC_SRCS}
    ${foo_RCC_SRCS}
)

SET(TARGET_H 
	CameraUpdateCallback.h
	ColladaVisitor
	Communicator.h
	Config_Memento.cpp
	Console
	FileStatusWidget.h
	Globals.h
	Graph_Evaluator.h
	MyGraphicsWindowQt
	MotionCallback.h
	UI_Console.h
	edge.h
	graphwidget.h
	hud.h
	node.h
	scene_model.h
	shader_defs.h
	viewingWindowQt
	window_capture_callback.h
	QCustomPlot/qcustomplot.h
)

#ADD_DEFINITIONS(-DUSE_QT4 -DQT_OPENGL_SUPPORT)

IF( QT4_FOUND )
  INCLUDE_DIRECTORIES( ${Jovian_SOURCE_DIR}/src ${OSG_INCLUDE_DIR}  ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ${QT_WIDGETS_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
ELSE()
  INCLUDE_DIRECTORIES( ${Jovian_SOURCE_DIR}/src ${OSG_INCLUDE_DIR}  ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )
ENDIF()

#### end var setup  ###
INCLUDE(OsgMacroUtils)
add_library( Jovian ${TARGET_SRC} ${TARGET_H} ) 

