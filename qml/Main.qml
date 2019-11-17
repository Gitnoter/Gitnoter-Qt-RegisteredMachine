import VPlayApps 1.0
import VPlay 2.0
import QtQuick 2.0
import QtQuick.Controls 2.0 as Quick2

import com.huyaohui.License 1.0

App {
    // You get free licenseKeys from https://v-play.net/licenseKey
    // With a licenseKey you can:
    //  * Publish your games & apps for the app stores
    //  * Remove the V-Play Splash Screen or set a custom one (available with the Pro Licenses)
    //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
    //licenseKey: "<generate one from https://v-play.net/licenseKey>"

    License {
        id: license
    }

    Storage {
        id: storage
        onStorageError: {
            errorData
            console.debug("there was an error:", errorData.message)
       }
    }

    Page {
        title: qsTr("Registered Machine")

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: dp(10)

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: dp(10)

                AppText {
                  anchors.verticalCenter: parent.verticalCenter
                  text: "选择平台: "
                }

                Quick2.ComboBox {
                  id: comboBox
                  implicitWidth: dp(120)
                  implicitHeight: dp(24) + topPadding + bottomPadding
                  padding: dp(12)
                  anchors.verticalCenter: parent.verticalCenter
                  model: [ "macOS", "Linux", "Windows" ]
                  // overwrite style for density independent sizes
                  delegate: Quick2.ItemDelegate {
                    width: comboBox.implicitWidth
                    height: comboBox.implicitHeight
                    padding: dp(12)
                    contentItem: AppText {
                      text: modelData
                      color: highlighted ? Theme.tintColor : Theme.textColor
                      wrapMode: Text.NoWrap
                    }
                    highlighted: comboBox.highlightedIndex == index
                  }

                  contentItem: AppText {
                    width: comboBox.width - comboBox.indicator.width - comboBox.spacing
                    text: comboBox.displayText
                    wrapMode: Text.NoWrap
                  }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: dp(10)

                AppText {
                  anchors.verticalCenter: parent.verticalCenter
                  text: "结束时间: "
                }

                AppTextField {
                  id: endTimeTextEdit

                  anchors.verticalCenter: parent.verticalCenter
                  y: dp(Theme.navigationBar.defaultBarItemPadding) * 0.5
                  width: dp(120)
                  height: dp(24)
                  placeholderText: "Enter text ..."
                  text: "Permanent"
                  font.pixelSize: dp(14)

                  verticalAlignment: Text.AlignVCenter
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: dp(10)

                AppText {
                  anchors.verticalCenter: parent.verticalCenter
                  text: "用户信息: "
                }

                AppTextField {
                  id: userInfoTextEdit

                  anchors.verticalCenter: parent.verticalCenter
                  y: dp(Theme.navigationBar.defaultBarItemPadding) * 0.5
                  width: dp(120)
                  height: dp(24)
                  placeholderText: "Enter text ..."
                  font.pixelSize: dp(14)

                  verticalAlignment: Text.AlignVCenter
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter

                AppButton {
                    id: button
                    flat: false
                    text: "生成许可证"

                    onClicked: {
                        var lidSequence = 100000
                        storage.getValue("lidSequence", function(result) {
                            lidSequence = result
                        })
                        lidSequence += 1
                        storage.setValue("lidSequence", lidSequence)
                        licenseText.text = license.createLicense(userInfoTextEdit.text, endTimeTextEdit.text, lidSequence, comboBox.currentText)
                        license.checkLicense(licenseText.text)
                    }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter

                AppTextEdit {
                    id: licenseText
                    anchors.verticalCenter: parent.verticalCenter
                    width: dp(200)
                    height: contentHeight + 2 * licenseText.textMargin
                    font.pixelSize: dp(14)
                    wrapMode: AppTextEdit.Wrap
                }
            }
        }
    }
}
