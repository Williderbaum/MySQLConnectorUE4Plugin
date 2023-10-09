#pragma once

#include  "mysql.h"

#include <cinttypes>
#define UI64FMTD "%" PRIu64

// Result field, used as an intermediary when collecting results from
// an MySQL query.
struct MySQLConnectorResultField
{
	
	MySQLConnectorResultField(const char* value, const char* name, enum_field_types type): m_value(value),m_name(name),m_type(type){}
	void SetValue(const char* value) { m_value = value; }
	float GetFloat() const { return m_value ? static_cast<float>(atof(m_value)) : 0.0f; }
	bool GetBool() const { return m_value ? atoi(m_value) > 0 : false; }
	int32 GetInt32() const { return m_value ? static_cast<int32>(atol(m_value)) : int32(0); }
	uint8 GetUInt8() const { return m_value ? static_cast<uint8>(atol(m_value)) : uint8(0); }
	uint16 GetUInt16() const { return m_value ? static_cast<uint16>(atol(m_value)) : uint16(0); }
	int16 GetInt16() const { return m_value ? static_cast<int16>(atol(m_value)) : int16(0); }
	uint32 GetUInt32() const { return m_value ? static_cast<uint32>(atoll(m_value)) : uint32(0); }
	uint64 GetUInt64() const
	{
		uint64 value = 0;
		if (!m_value || sscanf(m_value, UI64FMTD, &value) == -1)
			return 0;

		return value;
	}
	FString GetFString() const { return FString(m_value); }
	FString GetName() const { return FString(m_name); }

	std::string GetCppString() const
	{
		return m_value ? m_value : "";                    // std::string s = 0 have undefine result in C++
	}
	
	enum enum_field_types GetType() const { return m_type; }
	/*
	FString ToString()
	{
		if (Type == MySQLConnectorResultValueTypes::Varchar)
			return StringValue;
		if (Type == MySQLConnectorResultValueTypes::Int)
			return FString::Printf(TEXT("%i"), IntValue);
		return StringValue;
	}*/

private:
	const char* m_value;
	const char* m_name;
	enum enum_field_types m_type;
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
