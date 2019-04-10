
#ifndef FILE_STATUS_WIDGET_H
#define FILE_STATUS_WIDGET_H

#include <utility>
#include <vector>
#include <QButtonGroup>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpacerItem>
#include <QVBoxLayout>

typedef std::pair< QRadioButton*, QCheckBox* > WidgetPair;

class FileStatusWidget: public QObject
{
	Q_OBJECT

public:
	
	FileStatusWidget( QWidget* parent, QVBoxLayout* layout, QSpacerItem* spacer );
	~FileStatusWidget();
	
	void loadFile( QString fileName );
	
private slots:
	void fileSelected( int index );
	
signals:
	void fileLoaded( int index );
	void fileActivated( int index );
	
private:

	QWidget* _parent;
	QVBoxLayout* _fileGroupBoxLayout;
	QSpacerItem* _spacer;
	QButtonGroup* _grouper;
	std::vector< WidgetPair > _widgets;
	
};

#endif // FILE_STATUS_WIDGET_H

