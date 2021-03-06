#ifndef QT_CODE_NAVIGATOR_H
#define QT_CODE_NAVIGATOR_H

#include <QWidget>

#include "ErrorInfo.h"
#include "LocationType.h"
#include "MessageIndexingFinished.h"
#include "MessageListener.h"
#include "MessageSwitchColorScheme.h"
#include "MessageWindowFocus.h"
#include "QtCodeFileList.h"
#include "QtCodeFileSingle.h"
#include "QtThreadedFunctor.h"

class QLabel;
class QPushButton;
class QtSearchBarButton;
class SourceLocationCollection;
class SourceLocationFile;

class QtCodeNavigator
	: public QWidget
	, public MessageListener<MessageIndexingFinished>
	, public MessageListener<MessageSwitchColorScheme>
	, public MessageListener<MessageWindowFocus>
{
	Q_OBJECT

public:
	enum Mode
	{
		MODE_NONE,
		MODE_LIST,
		MODE_SINGLE
	};

	QtCodeNavigator(QWidget* parent = nullptr);
	virtual ~QtCodeNavigator();

	void addSnippetFile(const CodeFileParams& params);
	bool addSingleFile(const CodeFileParams& params, bool useSingleFileCache);
	void updateSourceLocations(const CodeSnippetParams& params);
	void updateReferenceCount(
		size_t referenceCount,
		size_t referenceIndex,
		size_t localReferenceCount,
		size_t localReferenceIndex);

	void clear();
	void clearSnippets();
	void clearFile();
	void clearCache();
	void clearSnippetReferences();

	void setMode(Mode mode);

	Id getSchedulerId() const override;
	void setSchedulerId(Id schedulerId);

	const std::set<Id>& getCurrentActiveTokenIds() const;
	void setCurrentActiveTokenIds(const std::vector<Id>& currentActiveTokenIds);

	const std::set<Id>& getCurrentActiveLocationIds() const;
	void setCurrentActiveLocationIds(const std::vector<Id>& currentActiveLocationIds);

	const std::set<Id>& getCurrentActiveLocalLocationIds() const;
	void setCurrentActiveLocalLocationIds(const std::vector<Id>& currentActiveLocalLocationIds);

	const std::set<Id>& getActiveTokenIds() const;
	void setActiveTokenIds(const std::vector<Id>& activeTokenIds);

	const std::set<Id>& getActiveLocalTokenIds() const;
	void setActiveLocalTokenIds(const std::vector<Id>& activeLocalTokenIds, LocationType locationType);

	const std::set<Id>& getFocusedTokenIds() const;
	void setFocusedTokenIds(const std::vector<Id>& focusedTokenIds);

	std::wstring getErrorMessageForId(Id errorId) const;
	void setErrorInfos(const std::vector<ErrorInfo>& errorInfos);

	bool hasErrors() const;
	size_t getFatalErrorCountForFile(const FilePath& filePath) const;

	bool isInListMode() const;
	bool hasSingleFileCached(const FilePath& filePath) const;

	void focusTokenIds(const std::vector<Id>& focusedTokenIds);
	void defocusTokenIds();

	void updateFiles();

	void refreshStyle();

	size_t findScreenMatches(const std::wstring& query);
	void activateScreenMatch(size_t matchIndex);
	void deactivateScreenMatch(size_t matchIndex);
	bool hasScreenMatches() const;
	void clearScreenMatches();

	void scrollTo(const CodeScrollParams& params, bool animated);

public slots:
	void scrolled(int value);

protected:
	void showEvent(QShowEvent* event) override;

private slots:
	void previousReference();
	void nextReference();

	void previousLocalReference();
	void nextLocalReference();

	void setModeList();
	void setModeSingle();

private:
	void handleMessage(MessageIndexingFinished* message) override;
	void handleMessage(MessageSwitchColorScheme* message) override;
	void handleMessage(MessageWindowFocus* message) override;

	QtThreadedLambdaFunctor m_onQtThread;

	QtCodeNavigateable* m_current;
	QtCodeFileList* m_list;
	QtCodeFileSingle* m_single;

	Mode m_mode;
	Mode m_oldMode;

	Id m_schedulerId;

	std::set<Id> m_currentActiveTokenIds;
	std::set<Id> m_currentActiveLocationIds;
	std::set<Id> m_currentActiveLocalLocationIds;

	std::set<Id> m_activeTokenIds;
	std::set<Id> m_activeLocalTokenIds;
	std::set<Id> m_focusedTokenIds;
	std::map<Id, ErrorInfo> m_errorInfos;

	QtSearchBarButton* m_prevReferenceButton;
	QtSearchBarButton* m_nextReferenceButton;
	QLabel* m_refLabel;

	QtSearchBarButton* m_prevLocalReferenceButton;
	QtSearchBarButton* m_nextLocalReferenceButton;
	QLabel* m_localRefLabel;

	QtSearchBarButton* m_listButton;
	QtSearchBarButton* m_fileButton;

	QFrame* m_separatorLine;

	CodeScrollParams m_scrollParams;

	std::vector<std::pair<QtCodeArea*, Id>> m_screenMatches;
	Id m_activeScreenMatchId = 0;
};

#endif	  // QT_CODE_NAVIGATOR_H
