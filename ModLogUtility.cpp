#include "ModLogUtility.h"
#include "HAL/IConsoleManager.h"
#include "HAL/FileManager.h"
#include "Misc/OutputDevice.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

DEFINE_LOG_CATEGORY(MODLOG_CATEGORY_NAME);

static TAutoConsoleVariable<int32> CVarLogLevel(
	TEXT(MODLOG_CONSOLE_VARIABLE_NAME), 1,
	TEXT("Sets log verbosity level: 0 = Error, 1 = Warning, 2 = Debug"),
	ECVF_Default
);

FModLogUtility FModLogUtility::Instance;

FModLogUtility& FModLogUtility::Get()
{
	return Instance;
}

void FModLogUtility::InitializeLog()
{
	if (bUseCustomLogFile)
	{
		FScopeLock Lock(&LogFileMutex);
		if (LogFilePath.IsEmpty())
		{
			LogFilePath = FPaths::Combine(
				FPlatformMisc::GetEnvironmentVariable(TEXT("LOCALAPPDATA")),
				TEXT(MODLOG_DIRECTORY),
				*FString::Printf(TEXT("%s.log"), MODLOG_MOD_NAME)
			);

			IFileManager::Get().Delete(*LogFilePath);
			RawLogMessage(FString::Printf(TEXT("%s Log Startup"), MODLOG_MOD_NAME));
		}
	}
}

void FModLogUtility::ShutdownLog()
{
	if (bUseCustomLogFile && !LogFilePath.IsEmpty())
	{
		RawLogMessage(TEXT("Module Log Shutdown"));
		LogFilePath.Empty();
	}
}

void FModLogUtility::SetUseCustomLogFile(bool bEnableCustomLogFile)
{
	bUseCustomLogFile = bEnableCustomLogFile;
}

void FModLogUtility::RawLogMessage(const FString& Message)
{
	FScopeLock Lock(&LogFileMutex);
	if (!LogFilePath.IsEmpty())
	{
		FString FormattedMessage = Message + LINE_TERMINATOR;
		FFileHelper::SaveStringToFile(FormattedMessage, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
	}
}

void FModLogUtility::LogMessage(const FString& Message, ELogLevel Level)
{
	int32 CurrentLogLevel = CVarLogLevel.GetValueOnAnyThread();
	if (static_cast<int32>(Level) >= CurrentLogLevel)
	{
		FString FormattedMessage;
		#if MODLOG_CLEAN_LOG
				FormattedMessage = Message; 
		#else
				const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
				FString Verbosity;
				switch (Level)
				{
				case ELogLevel::Error: Verbosity = TEXT("ERROR"); break;
				case ELogLevel::Warning: Verbosity = TEXT("WARNING"); break;
				case ELogLevel::Debug:
				default: Verbosity = TEXT("DEBUG"); break;
				}
				FormattedMessage = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Verbosity, *Message);
		#endif

		if (bUseCustomLogFile && !LogFilePath.IsEmpty())
		{
			RawLogMessage(FormattedMessage);
		}
		else
		{
			switch (Level)
			{
			case ELogLevel::Error:
				UE_LOG(MODLOG_CATEGORY_NAME, Error, TEXT("%s"), *FormattedMessage);
				break;
			case ELogLevel::Warning:
				UE_LOG(MODLOG_CATEGORY_NAME, Warning, TEXT("%s"), *FormattedMessage);
				break;
			case ELogLevel::Debug:
			default:
				UE_LOG(MODLOG_CATEGORY_NAME, Log, TEXT("%s"), *FormattedMessage);
				break;
			}
		}
	}
}

void UModLog::UseCustomLogFile(bool bEnableCustomLogFile)
{
	FModLogUtility::Get().SetUseCustomLogFile(bEnableCustomLogFile);
}

void UModLog::LogMessage(const FString& Message, EBPLogLevel Level)
{
	FModLogUtility& Logger = FModLogUtility::Get();
	Logger.LogMessage(Message, static_cast<ELogLevel>(Level));
}

void UModLog::InitializeLoggingModule()
{
	FModLogUtility::Get().InitializeLog();
	FModLogUtility::Get().LogMessage(FString::Printf(TEXT("%s Logging initialized"), MODLOG_MOD_NAME), ELogLevel::Debug);
}

void UModLog::ShutdownLoggingModule()
{
	FModLogUtility::Get().ShutdownLog();
}