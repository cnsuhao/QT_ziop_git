#include "StdAfx.h"
#include "ArchiveLogs.h"
#include "ZipArchive.h"

ArchiveLogs::ArchiveLogs(const std::string& debugFileName)
{
    m_logDir = boost::filesystem::system_complete(debugFileName).parent_path();
    m_archiveDir = m_logDir / makeArchiveDirName();
    moveLogsToArchiveDir();
}

void ArchiveLogs::moveLogsToArchiveDir()
{
    std::vector<boost::filesystem::path> logs = getFilesFromDirectory(m_logDir, ".log");

    if (logs.size())
    {
        boost::system::error_code ec;
        create_directory(m_archiveDir, ec);

        if (!ec)
        {
            for (boost::filesystem::path& log : logs)
            {
                rename(log, m_archiveDir / log.filename(), ec);

                if (ec)
                {
                    m_logsToCopy.push_back(log);
                }
            }

            boost::async(boost::bind(&ArchiveLogs::archive, this));
        }
    }
}

void ArchiveLogs::archive()
{
    boost::system::error_code ec;

    for (boost::filesystem::path& log : m_logsToCopy)
    {
        copy_file(log, m_archiveDir / log.filename(), ec);
    }

    std::vector<boost::filesystem::path> logs = getFilesFromDirectory(m_archiveDir);
    std::vector<CZipArchive> zips(logs.size());
    boost::latch latch(logs.size());

    for (size_t i = 0; i < logs.size(); ++i)
    {
        m_threads.submit(boost::bind(&ArchiveLogs::archiveOne, boost::ref(zips[i]), boost::ref(logs[i]), boost::ref(latch)));
    }

    latch.wait();
    CZipArchive zipAll;

    try
    {
        zipAll.Open((m_archiveDir.string() + ".zip").c_str(), CZipArchive::zipCreate);
        zipAll.SetCompressionMethod(CZipCompressor::methodWinZipAes);

        for (CZipArchive& zip : zips)
        {
            if (zip.GetCount())
            {
                zipAll.GetFromArchive(zip, 0);
            }

            zip.Close();
        }
    }
    catch (...)
    {
    }

    zipAll.Close();
    remove_all(m_archiveDir, ec);
}

void ArchiveLogs::archiveOne(CZipArchive& zip, const boost::filesystem::path& log, boost::latch& latch)
{
    boost::filesystem::path zippedLog(log.string() + ".zip");

    try
    {
        zip.Open(zippedLog.string().c_str(), CZipArchive::zipCreate);
        zip.SetCompressionMethod(CZipCompressor::methodWinZipAes);
        zip.AddNewFile(log.string().c_str(), -1, false);
    }
    catch (...)
    {
        remove(zippedLog);
    }

    latch.count_down();
    boost::system::error_code ec;
    remove(log, ec);
}

std::string ArchiveLogs::makeArchiveDirName()
{
    //TODO: make it simple
    char buf[100] = { 0 };
    time_t t = time(NULL);
    struct tm lt;
    ACE_OS::localtime_r(&t, &lt);
    ACE_OS::strftime(buf, 100, "ArchivedLogs_%Y-%m-%d_%H-%M-%S", &lt);
    return buf;
}

std::vector<boost::filesystem::path> ArchiveLogs::getFilesFromDirectory(const boost::filesystem::path& dir, const boost::filesystem::path& ext)
{
    std::vector<boost::filesystem::path> files;
    boost::filesystem::directory_iterator it(dir);
    boost::filesystem::directory_iterator end;

    for (; it != end; ++it)
    {
        if (is_regular_file(it->status()))
        {
            if (ext.empty() || ext == it->path().extension())
            {
                files.push_back(it->path());
            }
        }
    }

    return files;
}
