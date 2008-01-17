## ------------------------------------ ##
## liburbicore (Boost network module).  ##
## ------------------------------------ ##

# Add ASIO library.
AM_LDFLAGS += -lboost_system-gcc41 \
		-lssl

liburbicore_la_SOURCES +=			\
network/boost/connection.hh			\
network/boost/connection.hxx			\
network/boost/server.hh				\
network/boost/server.hxx			\
network/boost/server.cc				\
network/boost/fwd.hh

# SSL classes. Only if OpenSSL is available.
liburbicore_la_SOURCES +=			\
network/boost/server_ssl.cc			\
network/boost/server_ssl.hh			\
network/boost/connection_ssl.cc			\
network/boost/connection_ssl.hh

# Temporary file used to link with ucore-pc.
liburbicore_la_SOURCES +=			\
network/boost/network.cc			\
network/boost/network.hh			\
network/boost/network.hxx
