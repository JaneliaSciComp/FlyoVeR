
#include <iostream>

#include <QtCore/QFileInfo>
#include <QApplication>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <FileStatusWidget.h>

FileStatusWidget::FileStatusWidget( QWidget* parent, QVBoxLayout* layout, QSpacerItem* spacer ): _parent( parent ), _fileGroupBoxLayout( layout ), _spacer( spacer )
{
	// Lifted from QtDesigner
	QHBoxLayout* horizontalLayout_46 = new QHBoxLayout();
#ifndef Q_OS_MAC
	horizontalLayout_46->setSpacing(-1);
#endif

	QLabel* label_32 = new QLabel( _parent );
	label_32->setFrameShape(QFrame::NoFrame);
	label_32->setText(QApplication::translate("MainWindow", "Active", 0 ));

	horizontalLayout_46->addWidget(label_32);

	QLabel* label_33 = new QLabel( _parent );
	label_33->setText(QApplication::translate("MainWindow", "Loaded?", 0 ));

	horizontalLayout_46->addWidget(label_33);

	QLabel* label_34 = new QLabel( _parent );
	label_34->setText(QApplication::translate("MainWindow", "File Name", 0 ));

	horizontalLayout_46->addWidget(label_34);

	horizontalLayout_46->setStretch(1, 10);
	horizontalLayout_46->setStretch(2, 10);

	_fileGroupBoxLayout->insertLayout( _fileGroupBoxLayout->count() - 1, 
	                                   horizontalLayout_46 );

	_grouper = new QButtonGroup( _parent );
	connect( _grouper, SIGNAL( buttonClicked(int) ), this, SLOT( fileSelected(int) ) );
}

FileStatusWidget::~FileStatusWidget()
{
	_widgets.clear();
}
	
void
FileStatusWidget::loadFile( QString fileName )
{
	QFrame* frame_3 = new QFrame( _parent );
	frame_3->setFrameShape(QFrame::StyledPanel);
	frame_3->setFrameShadow(QFrame::Raised);

	QHBoxLayout* horizontalLayout_47 = new QHBoxLayout(frame_3);
#ifndef Q_OS_MAC
	horizontalLayout_47->setSpacing(-1);
#endif
	horizontalLayout_47->setContentsMargins(10, 0, 0, 0);

	QRadioButton* radioButton_4 = new QRadioButton( frame_3 );
	radioButton_4->setText(QString());
	radioButton_4->setChecked( true );
	
	horizontalLayout_47->addWidget(radioButton_4);

	QFrame* line = new QFrame(frame_3);
	line->setFrameShape(QFrame::VLine);
	line->setFrameShadow(QFrame::Sunken);

	horizontalLayout_47->addWidget(line);

	QCheckBox* checkBox_3 = new QCheckBox(frame_3);
	QFileInfo f( fileName );
	checkBox_3->setText( f.fileName() );

	WidgetPair wp( radioButton_4, checkBox_3 );
	_widgets.push_back( wp );
	_grouper->addButton( radioButton_4, _widgets.size() );
	
	horizontalLayout_47->addWidget(checkBox_3);

	QFrame* line_2 = new QFrame(frame_3);
	line_2->setFrameShape(QFrame::VLine);
	line_2->setFrameShadow(QFrame::Sunken);

	horizontalLayout_47->addWidget(line_2);

	horizontalLayout_47->setStretch(1, 1);
	horizontalLayout_47->setStretch(2, 1);

	_fileGroupBoxLayout->insertWidget( _fileGroupBoxLayout->count() - 1, frame_3 );
}

void
FileStatusWidget::fileSelected( int index )
{
	std::cout << "Selected id " << index << std::endl;
	emit fileActivated( index - 1 );
}

