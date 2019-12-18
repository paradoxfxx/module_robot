#让cmake自动去寻找Qt5
FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

#采用C++11标准
set(CMAKE_CXX_STANDARD 11)

#添加所有的源文件
SET(MOTOR_UI_SOURCES 
    motorcontrol.cpp
	main.cpp 
)

#添加所有的头文件
SET(MOTOR_UI_HEADERS 
    motorcontrol.h
)

#添加所有的.ui文件
SET(MOTOR_UI_FORMS 
    motorcontrol.ui
)

#添加资源文件
SET(MOTOR_UI_RESOURCES 
	./resources.qrc
)

#调用预编译器moc，需要使用 QT5_WRAP_CPP宏
QT5_WRAP_CPP(MOTOR_UI_HEADERS_MOC ${MOTOR_UI_HEADERS})
#使用uic处理.ui文件
QT5_WRAP_UI(MOTOR_UI_FORMS_HEADERS ${MOTOR_UI_FORMS})
#使用rcc处理.qrc文件
QT5_ADD_RESOURCES(MOTOR_UI_RESOURCES ${MOTOR_UI_RESOURCES})

#这些生成的中间文件都会在build目录下，这样的话，编译器则不能定位由uic程序产生的诸如_ui_mainwindow.h等文件。所以，我们需要把build目录添加到包含目录中
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#生成可执行文件
ADD_EXECUTABLE(ui_motor_control 
	${MOTOR_UI_SOURCES} 
	${MOTOR_UI_HEADERS_MOC} 
    ${MOTOR_UI_FORMS_HEADERS}
	${MOTOR_UI_RESOURCES} 
)

#为target添加需要链接的共享库
TARGET_LINK_LIBRARIES(ui_motor_control ${Qt5Widgets_LIBRARIES})