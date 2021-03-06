/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME qDataStoreWidget - Manage the main module's UI

#ifndef qDATASTOREGUI_H
#define qDATASTOREGUI_H

// Qt includes
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSignalMapper>
#include <QTime>
#include <QUrl>
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
#include <QWebFrame>
#endif
#include <QWidget>

// STD includes
#include <iostream>

#include "qSlicerDataStoreModuleExport.h"

class QNetworkReply;
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
class QWebView;
#else
class QWebEngineView;
#endif

namespace Ui {
class qDataStoreWidget;
}

class Q_SLICER_QTMODULES_DATASTORE_EXPORT qDataStoreWidget : public QWidget
{
    Q_OBJECT
    
public:  
  explicit qDataStoreWidget(QWidget *parent = 0);
  ~qDataStoreWidget();
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
  void setFailurePage(QWebView* webView);
#else
  void setFailurePage(QWebEngineView* webView);
#endif
  
  enum ColumnsIds
  {
  IconColumn = 0,
  NameColumn,
  ButtonsColumn,
  ColumnCount
  };
  
  // Add a new element in the Local datasets list
  void addNewTreeItem(QFileInfo fileName);
  
  // Remove a new element in the Local datasets list
  void deleteTreeItem(QString fileName);
    
public slots:
  //Save data as a file
  void downloaded(QNetworkReply* reply);
  //Tell webPage upload is finished
  void uploaded(QNetworkReply* reply);
  // Save the icon (thumbnail) ofthe dataset
  void iconDownloaded(QNetworkReply* reply);
  // Init download statistics
  void onStreamProgress(qint64 bytes, qint64 bytesTotal);
  // Load a dataset in the main Slicer application
  void loadDataset(QString fileName);
  // Delete a local file
  void deleteDataset(QString fileName);
  // Set the WebViews url
  void loadDataStoreURLs(QString);
  
  // Start the download of a dataset and its thumbnail
  void download(const QString& url, const QString& thumbnail);
  // Upload the current dataset to the selected URL
  void upload(const QString& url);
  // Cancel a download
  void cancelDownload();
  // Return the downlod speed and the progress
  QString getStreamStat();
  // Return the list of the downloaded items
  QString getDownloadedItems();
  
signals:
  void ScheduleLoad(QString);
  void ScheduleSave(QString);
    
protected slots:
  void onLoadStarted();
  void onLoadFinished(bool ok);
  void onLinkClicked(const QUrl& url);
  void initJavascript();
  void displayWindow();
    
private:
    Ui::qDataStoreWidget *ui;
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
    QWebFrame* downloadFrame;
    QWebFrame* uploadFrame;
    QWebView* DownloadPage;
    QWebView* UploadPage;
#else
    QWebEngineView* DownloadPage;
    QWebEngineView* UploadPage;
#endif
    QNetworkAccessManager networkDownloadManager;
    QNetworkAccessManager networkIconManager;
    QNetworkAccessManager networkUploadManager;
    QNetworkReply* CurrentReply;
    QTime StreamTime;
    QString StreamStat;
    QFile* StreamedFile;
    QString StreamId;
    bool DownloadCanceled;
    QSignalMapper LoadButtonMapper;
    QSignalMapper DeleteButtonMapper;
    
    QString DataSetDir;
    
    void saveDataset(QString fileName);
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
    void setDocumentWebkitHidden(QWebFrame* webFrame, bool value);
    QString evalJS(QWebFrame* webFrame, const QString &js);
#endif
};

#endif // qDATASTOREGUI_H
