#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED

#include "../settings/settings_definitions.h"
#include "../common/chrono.h"
#include "../common/helpers.h"
#include "../vapoursynth/vs_script_processor_structures.h"

#include <QObject>
#include <QUuid>
#include <QDateTime>
#include <QProcess>
#include <vector>

class SettingsManager;
class VSScriptLibrary;
class VapourSynthScriptProcessor;
class FrameHeaderWriter;

namespace vsedit
{

class Job : public QObject
{
	Q_OBJECT

public:

	Job(SettingsManager * a_pSettingsManager = nullptr,
		VSScriptLibrary * a_pVSScriptLibrary = nullptr,
		QObject * a_pParent = nullptr);
	virtual ~Job();

	enum class EncodingState
	{
		Idle,
		CheckingEncoderSanity,
		StartingEncoder,
		WritingHeader,
		WaitingForFrames,
		WritingFrame,
		EncoderCrashed,
		Finishing,
		Aborting,
	};

	static QString typeName(JobType a_type);

	static QString stateName(JobState a_state);

	virtual void start();
	virtual void pause();
	virtual void abort();

	virtual QUuid id() const;
	virtual bool setId(const QUuid & a_id);

	virtual JobType type() const;
	virtual bool setType(JobType a_type);

	virtual QString scriptName() const;
	virtual bool setScriptName(const QString & a_scriptName);

	virtual EncodingHeaderType encodingHeaderType() const;
	virtual bool setEncodingHeaderType(EncodingHeaderType a_headerType);

	virtual QString executablePath() const;
	virtual bool setExecutablePath(const QString & a_path);

	virtual QString arguments() const;
	virtual bool setArguments(const QString & a_arguments);

	virtual QString shellCommand() const;
	virtual bool setShellCommand(const QString & a_command);

	virtual JobState state() const;
	virtual bool setState(JobState a_state);

	virtual std::vector<QUuid> dependsOnJobIds() const;
	virtual bool setDependsOnJobIds(const std::vector<QUuid> & a_ids);

	virtual QString subject() const;

	virtual std::vector<vsedit::VariableToken> variables() const;

	virtual int framesProcessed() const;
	virtual int framesTotal() const;

signals:

	void signalStateChanged(JobState a_state);

protected:

	virtual void fillVariables();

	QUuid m_id;

	JobType m_type;
	JobState m_jobState;

	std::vector<QUuid> m_dependsOnJobIds;

	QDateTime m_timeStarted;
	QDateTime m_timeEnded;

	QString m_scriptName;

	EncodingType m_encodingType;
	EncodingHeaderType m_encodingHeaderType;
	QString m_executablePath;
	QString m_arguments;
	QString m_shellCommand;

	int m_firstFrame;
	int m_lastFrame;
	int m_framesProcessed;

	hr_time_point m_encodeStartTime;

	QProcess m_encoder;

	std::vector<char> m_framebuffer;

	int m_lastFrameProcessed;
	int m_lastFrameRequested;

	std::vector<vsedit::VariableToken> m_variables;

	EncodingState m_encodingState;

	size_t m_bytesToWrite;
	size_t m_bytesWritten;

	SettingsManager * m_pSettingsManager;

	VSScriptLibrary * m_pVSScriptLibrary;

	VapourSynthScriptProcessor * m_pVapourSynthScriptProcessor;

	const VSAPI * m_cpVSAPI;

	const VSVideoInfo * m_cpVideoInfo;

	FrameHeaderWriter * m_pFrameHeaderWriter;

	std::list<Frame> m_framesCache;
	size_t m_cachedFramesLimit;
};

}

#endif // JOB_H_INCLUDED