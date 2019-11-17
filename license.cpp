#include "license.h"

#include <QCryptographicHash>
#include <QTextStream>
#include <QDateTime>

License::License(QObject *parent) : QObject(parent)
{

}

QString License::createLicense(const QString &userInfo, const QString &endTime, QString lid, const QString &productType)
{
    lid += "-" + productType;

    const QString salt = "FsUr*5GDs9u#o0@Zvace5WAyAmVGX09u";
    const QString startTime = QDateTime::currentDateTime().toString(__LICENSE_TIMESTAMP_FROMAT__);
    const QString kernelType = getKernelTypeByName(productType);
    const QString sha512Part1 = QString(QCryptographicHash::hash(QString(startTime + userInfo + salt + lid + salt + kernelType).toUtf8(), QCryptographicHash::Sha512).toHex()).toUpper();
    const QString sha512Part2 = QString(QCryptographicHash::hash(QString(endTime + lid + salt + userInfo + salt + kernelType).toUtf8(), QCryptographicHash::Sha512).toHex()).toUpper();

    QString license = "—– BEGIN LICENSE —–\n";

    license += userInfo + "\n";
    license += startTime + "-" + endTime + "\n";
    license += "LID-" + lid + "\n";

    license += sha512Part1.mid(0, 8) + " " + sha512Part1.mid(8, 8) + " " + sha512Part1.mid(16, 8) + " " + sha512Part1.mid(24, 8) + "\n";
    license += sha512Part1.mid(32, 8) + " " + sha512Part1.mid(40, 8) + " " + sha512Part1.mid(48, 8) + " " + sha512Part1.mid(56, 8) + "\n";
    license += sha512Part1.mid(64, 8) + " " + sha512Part1.mid(72, 8) + " " + sha512Part1.mid(80, 8) + " " + sha512Part1.mid(88, 8) + "\n";
    license += sha512Part1.mid(96, 8) + " " + sha512Part1.mid(104, 8) + " " + sha512Part1.mid(112, 8) + " " + sha512Part1.mid(120, 8) + "\n";

    license += sha512Part2.mid(0, 8) + " " + sha512Part2.mid(8, 8) + " " + sha512Part2.mid(16, 8) + " " + sha512Part2.mid(24, 8) + "\n";
    license += sha512Part2.mid(32, 8) + " " + sha512Part2.mid(40, 8) + " " + sha512Part2.mid(48, 8) + " " + sha512Part2.mid(56, 8) + "\n";
    license += sha512Part2.mid(64, 8) + " " + sha512Part2.mid(72, 8) + " " + sha512Part2.mid(80, 8) + " " + sha512Part2.mid(88, 8) + "\n";
    license += sha512Part2.mid(96, 8) + " " + sha512Part2.mid(104, 8) + " " + sha512Part2.mid(112, 8) + " " + sha512Part2.mid(120, 8) + "\n";

    license += "—– END LICENSE —–";

    return license;
}


bool License::checkLicense(QString license)
{
    QString lid, userinfo, startTime, endTime, licenseKeyPart1, licenseKeyPart2 = "";
    QTextStream in(&license);
    QStringList stringList = {};

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }
        stringList.append(line);
    }

    if (stringList.length() != 13) {
        return false;
    }

    for (int i = 0; i < stringList.length(); ++i) {
        if (0 == i) {
            if (u8"—– BEGIN LICENSE —–" != stringList[i]) {
                return false;
            }
            continue;
        }
        else if (12 == i) {
            if (u8"—– END LICENSE —–" != stringList[i]) {
                return false;
            }
            continue;
        }
        else if (1 == i) {
            userinfo = stringList[i];
        }
        else if (2 == i) {
            QStringList list = stringList[i].split("-");
            startTime = list[0];
            endTime = list[1];
        }
        else if (3 == i) {
            if (!stringList[i].startsWith("LID-")) {
                return false;
            }

            lid = stringList[i].mid(4);
        }
        else if (4 <= i && i <= 7) {
            licenseKeyPart1 += stringList[i].replace(" ", "");
        }
        else if (8 <= i && i <= 11) {
            licenseKeyPart2 += stringList[i].replace(" ", "");
        }
    }

    const QString salt = "FsUr*5GDs9u#o0@Zvace5WAyAmVGX09u";
    const QString productType = QSysInfo::kernelType();
    const QString sha512Part1 = QString(QCryptographicHash::hash(QString(startTime + userinfo + salt + lid + salt + productType).toUtf8(), QCryptographicHash::Sha512).toHex()).toUpper();
    const QString sha512Part2 = QString(QCryptographicHash::hash(QString(endTime + lid + salt + userinfo + salt + productType).toUtf8(), QCryptographicHash::Sha512).toHex()).toUpper();

    if (sha512Part1 != licenseKeyPart1 || sha512Part2 != licenseKeyPart2) {
        return false;
    }

    int nowTimestamp = static_cast<int>(QDateTime::currentSecsSinceEpoch());
    int startTimestamp = QDateTime::fromString(startTime, __LICENSE_TIMESTAMP_FROMAT__).toTime_t();
    int endTimestamp = QDateTime::fromString(endTime, __LICENSE_TIMESTAMP_FROMAT__).toTime_t();
    endTimestamp = endTimestamp == -1 ? std::numeric_limits<int>::max() : endTimestamp;

    if (nowTimestamp < startTimestamp || nowTimestamp > endTimestamp || startTimestamp >= endTimestamp) {
        return false;
    }

    return true;
}

QString License::getKernelTypeByName(const QString &name)
{
    // "macOS", "Linux", "Windows"
    if ("macOS" == name) {
        return "darwin";
    }
    else if ("Linux" == name) {
        return "linux";
    }
    else if ("Windows" == name) {
        return "winnt";
    }

    return "";
}
