/*
 * Copyright (c) 2013 University of Washington
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mitch Watrous (watrous@u.washington.edu)
 */

#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include "ns3/file-aggregator.h"
#include "ns3/object-factory.h"
#include "ns3/probe.h"
#include "ns3/ptr.h"
#include "ns3/time-series-adaptor.h"

#include <map>
#include <string>

namespace ns3
{

/**
 * @ingroup stats
 * @brief Helper class used to put data values into a file.
 **/
class FileHelper
{
  public:
    /**
     * Constructs a file helper that will create a space separated file
     * named "file-helper.txt" unless it is later configured otherwise.
     */
    FileHelper();

    /**
     * @param outputFileNameWithoutExtension name of output file to
     * write with no extension
     * @param fileType type of file to write.
     *
     * Constructs a file helper that will create a file named
     * outputFileNameWithoutExtension plus possible extra information
     * from wildcard matches plus ".txt" with values printed as
     * specified by fileType.  The default file type is space-separated.
     */
    FileHelper(const std::string& outputFileNameWithoutExtension,
               FileAggregator::FileType fileType = FileAggregator::SPACE_SEPARATED);

    virtual ~FileHelper();

    /**
     * @param outputFileNameWithoutExtension name of output file to
     * write with no extension
     * @param fileType type of file to write.
     *
     * Configures file related parameters for this file helper so that
     * it will create a file named outputFileNameWithoutExtension plus
     * possible extra information from wildcard matches plus ".txt" with
     * values printed as specified by fileType.  The default file type
     * is space-separated.
     */
    void ConfigureFile(const std::string& outputFileNameWithoutExtension,
                       FileAggregator::FileType fileType = FileAggregator::SPACE_SEPARATED);

    /**
     * @param typeId the type ID for the probe used when it is created.
     * @param path Config path for underlying trace source to be probed
     * @param probeTraceSource the probe trace source to access.
     *
     * Creates output files generated by hooking the ns-3 trace source
     * with a probe, and then writing the values from the
     * probeTraceSource. The output file names will have the text stored
     * in m_outputFileNameWithoutExtension plus ".txt", and will consist
     * of the 'newValue' at each timestamp.
     *
     * This method will create one or more probes according to the TypeId
     * provided, connect the probe(s) to the trace source specified by
     * the config path, and hook the probeTraceSource(s) to the downstream
     * aggregator.
     *
     * If the config path has more than one match in the system
     * (e.g. there is a wildcard), then one output file for each match
     * will be created.  The output file names will contain the text in
     * m_outputFileNameWithoutExtension plus the matched characters for
     * each of the wildcards in the config path, separated by dashes,
     * plus ".txt".  For example, if the value in
     * m_outputFileNameWithoutExtension is the string
     * "packet-byte-count", and there are two wildcards in the path,
     * then output file names like "packet-byte-count-0-0.txt" or
     * "packet-byte-count-12-9.txt" will be possible as names for the
     * files that will be created.
     *
     * A fatal error will result if an unknown probe type is used.
     */
    void WriteProbe(const std::string& typeId,
                    const std::string& path,
                    const std::string& probeTraceSource);

    /**
     * @param adaptorName the timeSeriesAdaptor's name.
     *
     * @brief Adds a time series adaptor to be used to write the file.
     */
    void AddTimeSeriesAdaptor(const std::string& adaptorName);

    /**
     * @param aggregatorName the aggregator's name.
     * @param outputFileName name of the file to write.
     * @param onlyOneAggregator indicates if more than one aggregator
     * should be created or not.
     *
     * @brief Adds an aggregator to be used to write values to files.
     */
    void AddAggregator(const std::string& aggregatorName,
                       const std::string& outputFileName,
                       bool onlyOneAggregator);

    /**
     * @param probeName the probe's name.
     * @return Ptr to the probe
     * @brief Gets the specified probe.
     */
    Ptr<Probe> GetProbe(std::string probeName) const;

    /**
     * @return Ptr to a FileAggregator object
     * @brief Gets the single aggregator that is always constructed.
     *
     * This function is non-const because an aggregator may be lazily
     * created by this method.
     */
    Ptr<FileAggregator> GetAggregatorSingle();

    /**
     * @param aggregatorName name for aggregator.
     * @param outputFileName name of output file to write.
     * @return Ptr to a FileAggregator object
     * @brief Gets one of the multiple aggregators from the map.
     *
     * This function is non-const because an aggregator may be lazily
     * created by this method.
     */
    Ptr<FileAggregator> GetAggregatorMultiple(const std::string& aggregatorName,
                                              const std::string& outputFileName);

    /**
     * @param heading the heading string.
     *
     * @brief Sets the heading string that will be printed on the first
     * line of the file.
     *
     * Note that the heading string will only be printed if it has been
     * set by calling this function.
     */
    void SetHeading(const std::string& heading);

    /**
     * @param format the 1D format string.
     *
     * @brief Sets the 1D format string for the C-style sprintf()
     * function.
     */
    void Set1dFormat(const std::string& format);

    /**
     * @param format the 2D format string.
     *
     * @brief Sets the 2D format string for the C-style sprintf()
     * function.
     */
    void Set2dFormat(const std::string& format);

    /**
     * @param format the 3D format string.
     *
     * @brief Sets the 3D format string for the C-style sprintf()
     * function.
     */
    void Set3dFormat(const std::string& format);

    /**
     * @param format the 4D format string.
     *
     * @brief Sets the 4D format string for the C-style sprintf()
     * function.
     */
    void Set4dFormat(const std::string& format);

    /**
     * @param format the 5D format string.
     *
     * @brief Sets the 5D format string for the C-style sprintf()
     * function.
     */
    void Set5dFormat(const std::string& format);

    /**
     * @param format the 6D format string.
     *
     * @brief Sets the 6D format string for the C-style sprintf()
     * function.
     */
    void Set6dFormat(const std::string& format);

    /**
     * @param format the 7D format string.
     *
     * @brief Sets the 7D format string for the C-style sprintf()
     * function.
     */
    void Set7dFormat(const std::string& format);

    /**
     * @param format the 8D format string.
     *
     * @brief Sets the 8D format string for the C-style sprintf()
     * function.
     */
    void Set8dFormat(const std::string& format);

    /**
     * @param format the 9D format string.
     *
     * @brief Sets the 9D format string for the C-style sprintf()
     * function.
     */
    void Set9dFormat(const std::string& format);

    /**
     * @param format the 10D format string.
     *
     * @brief Sets the 10D format string for the C-style sprintf()
     * function.
     */
    void Set10dFormat(const std::string& format);

  private:
    /**
     * @param typeId the type ID for the probe used when it is created.
     * @param probeName the probe's name.
     * @param path Config path to access the probe
     *
     * @brief Adds a probe to be used to write values to files.
     */
    void AddProbe(const std::string& typeId, const std::string& probeName, const std::string& path);

    /**
     * @param typeId the type ID for the probe used when it is created.
     * @param matchIdentifier this string is used to make the probe's
     * context be unique.
     * @param path Config path to access the probe.
     * @param probeTraceSource the probe trace source to access.
     * @param outputFileNameWithoutExtension name of output file to
     * write with no extension
     * @param onlyOneAggregator indicates if more than one aggregator
     * should be created or not.
     *
     * @brief Connects the probe to the aggregator.
     *
     * A fatal error will result if an unknown probe type is used.
     */
    void ConnectProbeToAggregator(const std::string& typeId,
                                  const std::string& matchIdentifier,
                                  const std::string& path,
                                  const std::string& probeTraceSource,
                                  const std::string& outputFileNameWithoutExtension,
                                  bool onlyOneAggregator);

    /// Used to create the probes and collectors as they are added.
    ObjectFactory m_factory;

    /// The single aggregator that is always created in the constructor.
    Ptr<FileAggregator> m_aggregator;

    /// Maps aggregator names to aggregators when multiple aggregators
    /// are needed.
    std::map<std::string, Ptr<FileAggregator>> m_aggregatorMap;

    /// Maps probe names to probes.
    std::map<std::string, std::pair<Ptr<Probe>, std::string>> m_probeMap;

    /// Maps time series adaptor names to time series adaptors.
    std::map<std::string, Ptr<TimeSeriesAdaptor>> m_timeSeriesAdaptorMap;

    /// Number of file probes that have been created.
    uint32_t m_fileProbeCount;

    /// Determines the kind of file written by the aggregator.
    FileAggregator::FileType m_fileType;

    /// The name of the output file to created without its extension.
    std::string m_outputFileNameWithoutExtension;

    /// Indicates if the heading line for the file has been set.
    bool m_hasHeadingBeenSet;

    /// Heading line for the outputfile.
    std::string m_heading;

    std::string m_1dFormat;  //!< Format string for 1D format C-style sprintf() function.
    std::string m_2dFormat;  //!< Format string for 2D format C-style sprintf() function.
    std::string m_3dFormat;  //!< Format string for 3D format C-style sprintf() function.
    std::string m_4dFormat;  //!< Format string for 4D format C-style sprintf() function.
    std::string m_5dFormat;  //!< Format string for 5D format C-style sprintf() function.
    std::string m_6dFormat;  //!< Format string for 6D format C-style sprintf() function.
    std::string m_7dFormat;  //!< Format string for 7D format C-style sprintf() function.
    std::string m_8dFormat;  //!< Format string for 8D format C-style sprintf() function.
    std::string m_9dFormat;  //!< Format string for 9D format C-style sprintf() function.
    std::string m_10dFormat; //!< Format string for 10D format C-style sprintf() function.

}; // class FileHelper

} // namespace ns3

#endif // FILE_HELPER_H
