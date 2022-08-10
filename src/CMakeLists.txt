project(mechanicd)

include_directories(
    SYSTEM

    ${ANDROID_INCLUDE_DIRS}
)

add_executable(
    mechanicd

    listeners/power.cpp
    listeners/session.cpp
    listeners/session/logind.cpp
    listeners/service.cpp
    listeners/service/upstart.cpp
    tuners/schedtune.cpp
    tuners/renderthread.cpp
    dbus/scheduling.cpp

    main.cpp
)

target_link_libraries(
    mechanicd

    Qt5::Core
    Qt5::DBus
)

install(TARGETS mechanicd DESTINATION "${CMAKE_INSTALL_BINDIR}")
