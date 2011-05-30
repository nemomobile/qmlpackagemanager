
//From PackageKit::Enum
function needsRestartText(restart) {
    if (restart == undefined) return "";
    return ["Unknown", "No Restart", "Restart Session", "Restart System",
            "Restart Security Session", "Restart Security System"][restart];
}

function groupName(group) {
    if (group == undefined) return "";
    return ["Unknown", "Accessibility", "Accessories", "AdminTools", "Communication",
            "DesktopGnome", "DesktopKde", "DesktopOther", "DesktopXfce", "Education",
            "Fonts", "Games", "Graphics", "Internet", "Legacy", "Localization",
            "Maps", "Multimedia", "Network", "Office", "Other", "PowerManagement",
            "Programming", "Publishing", "Repos", "Security", "Servers", "System",
            "Virtualization", "Science", "Documentaton", "Electronics", "Collections",
            "Vendor", "Newest"][group];
}

function updateStateName(state) {
    if (state == undefined) return "";
    return ["Unknown", "Stable", "Unstable", "Testing"][state];
}

function errorCodeName(error) {
    if (error == undefined) return "";
    return ["UnknownError","ErrorOom","ErrorNoNetwork","ErrorNotSupported","ErrorInternalError",
            "ErrorGpgFailure","ErrorPackageIdInvalid","ErrorPackageNotInstalled",
            "ErrorPackageNotFound","ErrorPackageAlreadyInstalled","ErrorPackageDownloadFailed",
            "ErrorGroupNotFound","ErrorGroupListInvalid","ErrorDepResolutionFailed",
            "ErrorFilterInvalid","ErrorCreateThreadFailed","ErrorTransactionError",
            "ErrorTransactionCancelled","ErrorNoCache","ErrorRepoNotFound",
            "ErrorCannotRemoveSystemPackage","ErrorProcessKill","ErrorFailedInitialization",
            "ErrorFailedFinalise","ErrorFailedConfigParsing","ErrorCannotCancel","ErrorCannotGetLock",
            "ErrorNoPackagesToUpdate","ErrorCannotWriteRepoConfig","ErrorLocalInstallFailed",
            "ErrorBadGpgSignature","ErrorMissingGpgSignature","ErrorCannotInstallSourcePackage",
            "ErrorRepoConfigurationError","ErrorNoLicenseAgreement","ErrorFileConflicts",
            "ErrorPackageConflicts","ErrorRepoNotAvailable","ErrorInvalidPackageFile",
            "ErrorPackageInstallBlocked","ErrorPackageCorrupt","ErrorAllPackagesAlreadyInstalled",
            "ErrorFileNotFound","ErrorNoMoreMirrorsToTry","ErrorNoDistroUpgradeData",
            "ErrorIncompatibleArchitecture","ErrorNoSpaceOnDevice","ErrorMediaChangeRequired",
            "ErrorNotAuthorized","ErrorUpdateNotFound","ErrorCannotInstallRepoUnsigned",
            "ErrorCannotUpdateRepoUnsigned","ErrorCannotGetFilelist","ErrorCannotGetRequires",
            "ErrorCannotDisableRepository","ErrorRestrictedDownload","ErrorPackageFailedToConfigure",
            "ErrorPackageFailedToBuild","ErrorPackageFailedToInstall","ErrorPackageFailedToRemove",
            "ErrorUpdateFailedDueToRunningProcess","ErrorPackageDatabaseChanged",
            "ErrorProvideTypeNotSupported","ErrorInstallRootInvalid","ErrorCannotFetchSources"][error];
}

function infoText(info) {
    if (info == undefined) return "";
    return ["Unknown", "Installed", "Available", "Low", "Enhanchement", "Normal",
            "Bug Fix", "Important", "Security", "Blocked", "Downloading", "Updating",
            "Installing", "Removing", "Cleanup", "Obsoleting", "Collection Installed",
            "Collection Available", "Finished", "Reinstalling", "Downgrading", "Preparing",
            "Decompressing"][info]
}

function statusTextLiteral(status) {
    if (status == undefined) return "";
    return ["UnknownStatus", "StatusWait", "StatusSetup", "StatusRunning", "StatusQuery",
            "StatusInfo", "StatusRemove", "StatusRefreshCache", "StatusDownload", "StatusInstall",
            "StatusUpdate", "StatusCleanup", "StatusObsolete", "StatusDepResolve", "StatusSigCheck",
            "StatusRollback", "StatusTestCommit", "StatusCommit", "StatusRequest", "StatusFinished",
            "StatusCancel", "StatusDownloadRepository", "StatusDownloadPackagelist",
            "StatusDownloadFilelist", "StatusDownloadChangelog", "StatusDownloadGroup",
            "StatusDownloadUpdateinfo", "StatusRepackaging", "StatusLoadingCache",
            "StatusScanApplications", "StatusGeneratePackageList", "StatusWaitingForLock",
            "StatusWaitingForAuth", "StatusScanProcessList", "StatusCheckExecutableFiles",
            "StatusCheckLibraries", "StatusCopyFiles"][status]
}

function statusText(status) {
    if (status == undefined) return "";
    return ["Unknown State", "Waiting", "Starting", "Executing", "Querying",
            "Reading Info", "Removing", "Refreshing Cache", "Downloading", "Installing",
            "Updating", "Cleaning up", "Obsoleting", "Resolving Dependencies", "Checking Signature",
            "Rolling back", "Testing Commit", "Committing", "Requesting", "Finished",
            "Canceling", "Downloading Repository", "Downloading Package List",
            "Downloading File List", "Downloading Changelog", "Downloading Group Info",
            "Downloading Update Info", "Repackaging", "Loading Cache",
            "Scanning Applications", "Generating Package List", "Waiting For Lock",
            "Waiting For Authentication", "Scanning Process List", "Checking Executable Files",
            "Checking Libraries", "Copying Files"][status]
}

function transactionInfo(t) {
    if (t == undefined) return ""
    return t.state + " "
            + statusText(t.status) + " "
            + (t.percentage <= 100? t.percentage + " ": "")
            + (t.subpercentage <= 100? t.subpercentage + " ": "")
            + t.currentPackage + " "
            + (t.state == "error"? errorCodeName(t.errorCode) + " " + t.errorText + " ": "")
}
