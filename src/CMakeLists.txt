set(nexus-srcs
	client.cc
	connection.cc
	connection_state.cc
	engine.cc
	error.cc
	global.cc
	server.cc
	settings.cc
	socket.cc
	stream.cc
	stream_state.cc)

find_package(lsquic REQUIRED)
find_package(Boost REQUIRED)

add_library(nexus ${nexus-srcs})
target_link_libraries(nexus PUBLIC nexus-headers lsquic::lsquic Boost::headers)
install(TARGETS nexus LIBRARY DESTINATION lib)
