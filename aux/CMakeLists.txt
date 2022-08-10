install(FILES upstart/mechanicd.conf DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/init)
install(FILES systemd/mechanicd.service DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/systemd/system)
install(FILES dbus/org.halium.mechanicd.conf DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/dbus-1/system.d)
install(FILES dbus/org.halium.mechanicd.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/system-services)
