/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <fstream>

#include <QtCore/QVector>

#include <QGraphicsView>

#include <Globals.h>

class Node;
class ViewingWindowQt;

class GraphWidget : public QGraphicsView
{
	Q_OBJECT

	public:
	GraphWidget( QWidget* parent = 0 );

	void itemMoved();
	
	QPointF evaluate( float u, float v );
	void compute_basis_matrix();
	void set_viewer( ViewingWindowQt* viewer ) { _viewer = viewer; }
	
	void distributeHorizontally();
	void distributeVertically();
	void reset();
	void smooth();
	void linearizeEdges();
	
	void save( GraphPositions& gp );
	void load( GraphPositions const& gp );
	
	QVector< QVector< Node* > > nodes;
	float push_weight, pull_weight;
	
protected:
	void keyPressEvent(QKeyEvent *event);
	void timerEvent(QTimerEvent *event);
	void wheelEvent(QWheelEvent *event);
	void drawBackground(QPainter *painter, const QRectF &rect);

	void scaleView(qreal scaleFactor);
	void linearizeEdgeInX( Node* n1, Node* n2, Node* n3, Node* n4 );
	void linearizeEdgeInY( Node* n1, Node* n2, Node* n3, Node* n4 );

private:
	int timerId;
	QPointF _origin;
	QVector< QVector< qreal > > _basis;
	QVector< QVector< QPointF > > _mesh;
	ViewingWindowQt* _viewer;
};

#endif
