#pragma once

class CZipArchive;

struct ArchiveLogs
{
public:

    ArchiveLogs(const std::string& m_debugFileName);

protected:

    void archive();
    void moveLogsToArchiveDir();
    static void archiveOne(CZipArchive& zip, const boost::filesystem::path& log, boost::latch& latch);
    static std::string makeArchiveDirName();
    static std::vector<boost::filesystem::path> getFilesFromDirectory(const boost::filesystem::path& dir, const boost::filesystem::path& ext = "");

protected:

    boost::filesystem::path m_logDir;
    boost::filesystem::path m_archiveDir;
    std::vector<boost::filesystem::path> m_logsToCopy;
    boost::basic_thread_pool m_threads;
};
