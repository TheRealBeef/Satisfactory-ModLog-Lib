#pragma once

#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "ModLogUtility.generated.h"

///////////////////////////////////////////////////////////////////////////
/// All the configuration is done with these defines - you need to put	///
/// your mod information in here. Don't forget to update the MYMOD_API	///
/// down below as well													///
///////////////////////////////////////////////////////////////////////////
#define MODLOG_CATEGORY_NAME ModLog
#define MODLOG_CONSOLE_VARIABLE_NAME "ModLog.LogLevel"
#define MODLOG_DIRECTORY "FactoryGame/Saved/Logs"
#define MODLOG_FILE_NAME TEXT("ModLog")
#define MODLOG_MOD_NAME TEXT("DefaultMod")
#define MODLOG_USE_CUSTOM_LOGGING true	// Set to false to disable writing to custom file for release version of mod
#define MODLOG_CLEAN_LOG true			// Set to false to add in the timestamp and verbosity information


// Don't change this line
DECLARE_LOG_CATEGORY_EXTERN(MODLOG_CATEGORY_NAME, Log, All);

// Maybe there's a more elegant way than duplicate enums, but this is prototype
enum class ELogLevel
{
	Debug = 2,
	Warning = 1,
	Error = 0
};
UENUM(BlueprintType)
enum class EBPLogLevel : uint8
{
	Error UMETA(DisplayName = "Error"),
	Warning UMETA(DisplayName = "Warning"),
	Debug UMETA(DisplayName = "Debug")
};


class FModLogUtility
{
public:
	///////////////////////////////////////////////////////////////////////////
	/// This LogMessage() is how you will log messages in C++			    ///
	/// Example usage:														///
	/// FModLogUtility::Get().LogMessage("Your Message", ELogLevel::Debug); ///
	///////////////////////////////////////////////////////////////////////////
	void LogMessage(const FString& Message, ELogLevel Level);

	///////////////////////////////////////////////////////////////////////////
	/// Don't call these directly										    ///
	static FModLogUtility& Get();                                           ///
	void InitializeLog();                                                   ///
	void ShutdownLog();                                                     ///
	void SetUseCustomLogFile(bool bEnableCustomLogFile);                    ///
	///////////////////////////////////////////////////////////////////////////

private:
	FModLogUtility() = default;
	FModLogUtility(const FModLogUtility&) = delete;
	FModLogUtility& operator=(const FModLogUtility&) = delete;
	bool bUseCustomLogFile = MODLOG_USE_CUSTOM_LOGGING;
	FString LogFilePath;
	FCriticalSection LogFileMutex;
	static FModLogUtility Instance;

	void RawLogMessage(const FString& Message);
	static const FLogCategoryBase* LogCategory;
};


/////////////////////////////////////////////////////////////////////////
/// CHANGE MYMOD_API HERE TO YOUR MOD'S API INSTEAD					  ///
/////////////////////////////////////////////////////////////////////////
UCLASS()
class MYMOD_API UModLog : public UObject
{
	GENERATED_BODY()

public:
	/////////////////////////////////////////////////////////////////////////
	/// Start here: You need to call this InitializeLoggingModule()       ///
	/// whether you're using C++ logging or blueprint logging             ///
	/////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = MODLOG_MOD_NAME)
	static void InitializeLoggingModule();

	/////////////////////////////////////////////////////////////////////////
	/// Called to shut-down the logging module							  ///
	/////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = MODLOG_MOD_NAME)
	static void ShutdownLoggingModule();

	/////////////////////////////////////////////////////////////////////////
	/// Set this true in Blueprint to write to custom file, or false to   ///
	/// write to normal UE_Log output. It will override you have set      ///
	/// here in the C++ so it's kind of the "master" control              ///
	/////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = MODLOG_MOD_NAME)
	static void UseCustomLogFile(bool bEnableCustomLogFile);
	
	/////////////////////////////////////////////////////////////////////////
	/// Self-explanatory - logs messages (called in blueprints)			  ///
	/////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = MODLOG_MOD_NAME)
	static void LogMessage(const FString& Message, EBPLogLevel Level);
};
