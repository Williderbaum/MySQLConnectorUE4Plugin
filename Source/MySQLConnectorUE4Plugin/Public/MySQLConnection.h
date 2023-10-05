#pragma once

#include  "mysql.h"


// A few things for internal use here.
namespace MySQLConnectorResultValueTypes
{
	enum MySQLConnectorResultValType
	{
		Int,
		Varchar,
		UnsupportedValueType
	};
}


// Result field, used as an intermediary when collecting results from
// an MySQL query.
struct MySQLConnectorResultField
{
	FString StringValue;
	double DoubleValue;
	int64 IntValue;

	FString Name;
	MySQLConnectorResultValueTypes::MySQLConnectorResultValType Type;

	FString ToString()
	{
		if (Type == MySQLConnectorResultValueTypes::Varchar)
			return StringValue;
		if (Type == MySQLConnectorResultValueTypes::Int)
			return FString::Printf(TEXT("%i"), IntValue);
		return StringValue;
	}
};

struct MySQLConnectorResultValue
{
	TArray<MySQLConnectorResultField> Fields;
};

// The internal result object.
struct MySQLConnectorQueryResult
{
	bool Success;
	FString ErrorMessage;
	TArray<MySQLConnectorResultValue> Results;
};





class MYSQLCONNECTORUE4PLUGIN_API FMySQLConnection
{
	
public:

    MYSQL* globalCon = nullptr;
   
	bool MySQLCheckConnection();

	// checks if the connection is valid and closes it, resets the pointers to nullptr on success
	// true on success, false in case the connection is not established or the argument is nullptr
	bool MySQLCloseConnection();

	bool MySQLInitConnection(FString Host, FString UserName, FString UserPassword, FString DatabaseName);

	bool MySQLConnectorExecuteQuery(FString Query);

	/** Get data from the database using a select statement and return the rows. */
	MySQLConnectorQueryResult RunQueryAndGetResults(const FString& Query);
};
