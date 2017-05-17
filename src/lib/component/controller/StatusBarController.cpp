#include "component/controller/StatusBarController.h"

#include "component/view/StatusBarView.h"
#include "data/access/StorageAccess.h"
#include "utility/logging/logging.h"

StatusBarController::StatusBarController(StorageAccess* storageAccess)
	: m_storageAccess(storageAccess)
{
}

StatusBarController::~StatusBarController()
{
}

StatusBarView* StatusBarController::getView()
{
	return Controller::getView<StatusBarView>();
}

void StatusBarController::clear()
{
	getView()->setErrorCount(ErrorCountInfo());
}

void StatusBarController::handleMessage(MessageClearErrorCount* message)
{
	getView()->setErrorCount(ErrorCountInfo());
}

void StatusBarController::handleMessage(MessageFinishedParsing* message)
{
	ErrorCountInfo errorCount = m_storageAccess->getErrorCount();
	getView()->setErrorCount(errorCount);
}

void StatusBarController::handleMessage(MessagePingReceived* message)
{
	std::string status = "No IDE connected";

	if (message->ideName.length() > 0)
	{
		status = "Connected to ";
		status += message->ideName;
	}

	getView()->showIdeStatus(status);
}

void StatusBarController::handleMessage(MessageRefresh* message)
{
	getView()->setErrorCount(m_storageAccess->getErrorCount());
}

void StatusBarController::handleMessage(MessageShowErrors* message)
{
	if (message->errorId || message->isReplayed())
	{
		return;
	}

	getView()->setErrorCount(message->errorCount);
}

void StatusBarController::handleMessage(MessageStatus* message)
{
	setStatus(message->status, message->isError, message->showLoader);
}

void StatusBarController::setStatus(const std::string& status, bool isError, bool showLoader)
{
	if (!status.empty())
	{
		LOG_INFO_STREAM(<< "STATUS " << status);
	}

	getView()->showMessage(status, isError, showLoader);

}
