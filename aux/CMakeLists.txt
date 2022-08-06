install(FILES upstart/mechanicd.conf DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/init)
install(FILES dbus/org.halium.mechanicd.conf DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/dbus-1/system.d)
