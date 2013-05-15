// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/asio/io_service.hpp>
#include <libcql/cql.hpp>
#include <libcql/cql_error.hpp>
#include <libcql/cql_client.hpp>
#include <libcql/cql_client_factory.hpp>
#include <libcql/cql_result.hpp>



	void
query_callback(cql::cql_client_t& client,
               int8_t stream,
               cql::cql_result_t* result)
{
    // close the connection
	std::string key;
    std::string value;

    while (result->next() ) {
		result->get_string(0,key);
		result->get_string(1,value);
    	std::cout << "\"" << key << "\":  \"" << value << "\"\n";
    }

    client.close();
}

void
message_errback(cql::cql_client_t& client,
                int8_t stream,
                const cql::cql_error_t& err)
{
    std::cerr << "ERROR " << err.message << std::endl;
}

void connection_errback(cql::cql_client_t& client, const cql::cql_error_t& err) {
	std::cout << "ERROR: " << err.message;
}

	void
connect_callback(cql::cql_client_t& client)
{
    // Called after a successfull connection, or
    // if using SSL it's called after a successfull handshake.

    // perform a query
    client.query("select * from nhanes.\"textTable\";",
                 cql::CQL_CONSISTENCY_ALL,
                 &query_callback,
                 &message_errback);
}



int _tmain(int argc, _TCHAR* argv[])
{

			boost::asio::io_service io_service;

			// instantiate a client
			std::auto_ptr<cql::cql_client_t> client(cql::cql_client_factory_t::create_cql_client_t(io_service));

			// Connect to localhost
			client->connect("10.0.2.2", 9042, &connect_callback, &connection_errback);

			// Start the event loop
			io_service.run();


	return 0;
}

