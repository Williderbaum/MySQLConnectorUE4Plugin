#include "MySQLConnection.h"
#include "MySQLConnectorUE4Plugin.h"


bool FMySQLConnection::MySQLCheckConnection()
{
	if (globalCon)
	{
		return mysql_ping(globalCon) == 0;
	}
	return false;
}

bool FMySQLConnection::MySQLCloseConnection()
{
	
	if (MySQLCheckConnection())
	{
		mysql_close(globalCon);
		globalCon = nullptr;
		mysql_library_end();
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLCloseConnection: Connection is valid but Server does no respond!"));
	}
	
	
	return false;
}

bool FMySQLConnection::MySQLInitConnection(FString Host, FString UserName, FString UserPassword, FString DatabaseName)
{
	std::string HostString(TCHAR_TO_UTF8(*Host));
	std::string UserNameString(TCHAR_TO_UTF8(*UserName));
	std::string UserPasswordString(TCHAR_TO_UTF8(*UserPassword));
	std::string DatabaseNameString(TCHAR_TO_UTF8(*DatabaseName));

	if (mysql_library_init(0, nullptr, nullptr) != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: FAILED TO INIT mysql library"));
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("FAILED TO INIT mysql library"));
		mysql_library_end();
		return false;
	}

	globalCon = mysql_init(nullptr);
	if (!globalCon)
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: FAILED TO INIT connection"));
		mysql_library_end();
		return false;
	}

	if (!mysql_real_connect(globalCon,
		HostString.c_str(),
		UserNameString.c_str(),
		UserPasswordString.c_str(),
		DatabaseNameString.c_str(),
		0, nullptr, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: Failed to Connect to Database!"));
		MySQLCloseConnection();
		return false;
	}

	if (mysql_set_character_set(globalCon, "utf8") != 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("MySQLConnector: Can't set UTF-8 with mysql_set_character_set"));
		UE_LOG(LogTemp, Error, TEXT("MySQLInitConnection: Can't set UTF-8 with mysql_set_character_set"));
		MySQLCloseConnection();
		return false;
	}

	return true;
}

bool FMySQLConnection::MySQLConnectorExecuteQuery(FString Query)
{
	if (!globalCon)
	{
		UE_LOG(LogTemp, Error, TEXT("Connection is NULL!"));
		return false;
	}

	std::string MyStdString(TCHAR_TO_UTF8(*Query));

	//if (mysql_query(con, "INSERT INTO `test`.`t1` (`Qwe`) VALUES ('test ');")) {
	if (mysql_query(globalCon, MyStdString.c_str()))
	{
		return false;
	}
	return true;
}

MySQLConnectorQueryResult FMySQLConnection::RunQueryAndGetResults(const FString& Query)
{
	MySQLConnectorQueryResult resultOutput;
	
	if (!MySQLCheckConnection())
	{
		UE_LOG(LogTemp, Error, TEXT("Not connected!"));
		resultOutput.ErrorMessage = "Not connected!";
		resultOutput.Success = false;
		return resultOutput;
	};

	//////////////////////////////////////////////////////////////////////////
	// Get and assign the data
	//////////////////////////////////////////////////////////////////////////

	std::string MyStdString(TCHAR_TO_UTF8(*Query));

	if (mysql_query(globalCon, MyStdString.c_str())) // "SELECT * FROM Cars"
	{
		//finish_with_error(con);
	}

	MYSQL_RES* result = mysql_store_result(globalCon);

	if (!result)
	{
		//finish_with_error(con);		
		UE_LOG(LogTemp, Error, TEXT("Result is NULL!"));
		resultOutput.ErrorMessage = "Result is NULL!";
		resultOutput.Success = false;
		return resultOutput;
	}

	int num_fields = mysql_num_fields(result);

	TArray<int> fieldTypes;
	TArray<FString> fieldNames;

	MYSQL_FIELD* fields;
	fields = mysql_fetch_fields(result);
	if (fields)
	{
		for (int i = 0; i < num_fields; i++)
		{
			FString NewString = FString::FromInt(fields[i].type);
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("MySQLConnector: Type is:") + NewString);

			fieldTypes.Add(fields[i].type);
			fieldNames.Add(UTF8_TO_TCHAR(fields[i].name));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and assign the data
	//////////////////////////////////////////////////////////////////////////

	TArray<MySQLConnectorResultValue> resultRows;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		MySQLConnectorResultValue rowVal;

		for (int i = 0; i < num_fields; i++)
		{
			MySQLConnectorResultField val;

			FString columnNameStr = fieldNames[i];
			val.Name = columnNameStr;

			FString fieldValueStr = (UTF8_TO_TCHAR(row[i]));

			switch (fieldTypes[i])
			{
			case enum_field_types::MYSQL_TYPE_LONG:
				val.Type = MySQLConnectorResultValueTypes::Int;
				val.IntValue = FCString::Atoi(*fieldValueStr);
				break;
			case enum_field_types::MYSQL_TYPE_VAR_STRING:
				val.Type = MySQLConnectorResultValueTypes::Varchar;
				val.StringValue = fieldValueStr;
				break;
			default:
				val.Type = MySQLConnectorResultValueTypes::UnsupportedValueType;
				val.IntValue = FCString::Atoi(*fieldValueStr);
				val.StringValue = fieldValueStr;
			}

			rowVal.Fields.Add(val);
		}

		resultRows.Add(rowVal);
	}

	mysql_free_result(result);

	resultOutput.Results = resultRows;
	resultOutput.Success = true;

	return resultOutput;
}
