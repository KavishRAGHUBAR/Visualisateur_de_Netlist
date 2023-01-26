#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include <QString>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include "Shape.h"


namespace Netlist
{

using namespace std;


ostream& operator<< ( ostream& o, const QRect& rect )
{
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
}


ostream& operator<< ( ostream& o, const QPoint& p )
{
    o << "<QRect x:" << p.x() << " y:" << p.y() << ">";
    return o;
}//Operateur de ostream



CellWidget::CellWidget ( QWidget* parent )//CTOR de cellwidget
    : QWidget(parent)
    , cell_  (NULL)
    , viewport_ (Box(0,0,500,500))//Initialiser le viewport en forme de box
{
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setAttribute    ( Qt::WA_StaticContents );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
}


CellWidget::~CellWidget ()
{ }


void  CellWidget::setCell ( Cell* cell )
{
    cell_ = cell;
    repaint();
}


QSize  CellWidget::minimumSizeHint () const //on ne pourra pas resize moins que 500x500 pixels
{
    return QSize(500,500);
}


void  CellWidget::resizeEvent ( QResizeEvent* event )
{
    const QSize & size = event->size();
    viewport_.setX2( viewport_.getX1()+size.width());
    viewport_.setY1( viewport_.getY2()-size.height());
    //cerr << " CellWidget::resizeEvent() viewport_:"<< viewport_ << endl ;

}


void  CellWidget::paintEvent ( QPaintEvent* event )
{

    QFont  bigFont = QFont( "URW Bookman L", 36 );

    QString cellName = "NULL";
    if (cell_) cellName = cell_->getName().c_str();

    QPainter painter(this);
    painter.setFont      ( bigFont );
    painter.setBackground( QBrush( Qt::black ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );

    int frameWidth  = 460;
    int frameHeight = 100;
    QRect nameRect ( (size().width ()-frameWidth )/2
                     , (size().height()-frameHeight)/2
                     , frameWidth
                     , frameHeight
                   );






    //painter.drawRect ( rect2 );
    query(1,painter);//Appeler la methode de query, c'est qui est la methode de painter

}


void CellWidget::query ( unsigned int flags , QPainter & painter )
{
    if (( not cell_ ) or ( not flags )) return ; //cell_ ou flag_ nuls, on sort de la fonction
    const vector < Instance* > &instances = cell_ -> getInstances(); //recupere tous les instances
    const vector < Shape* > &shapes = cell_-> getSymbol()->getShapes(); //recupere tous les shapes




    if(instances.size()!=0)//Si l'instance n'est pas nul donc il existe les instances
    {

        const vector < Term* > &terms = cell_-> getTerms();//Recupere les terms
        for (size_t l=0; l< terms.size(); ++l) //on va parcourir les terms de cell_(exemples a,b,sout,cout)
        {
            QString name=terms[l]->getName().c_str();//Recupere les noms des terms en forme de Qstring
            //cerr<<name.toStdString ()<<endl;
            Point termpoint=terms[l]->getPosition();//Recupere les positions des terms
            //cerr<<"x:"<<termpoint.getX()<<endl;
            //cerr<<"y:"<<termpoint.getY()<<endl;

            QFont font=painter.font();//Mettre le painter en layout plus haut
            font.setPixelSize(20); //taille du texte
            painter.setPen ( QPen ( Qt::white , 1 ) ); //choix de couleur blanche, d'epaisseur 1 pixel
            painter.setFont(font);
            painter.drawText(xToScreenX(termpoint.getX()+10),yToScreenY(termpoint.getY()+10),name);//Appeler la methode de drawtext pour dessiner les noms

            Term::Direction direct=terms[l]->getDirection();//Recupere les directions des terms pour dessiner les tags des entrees et sorties
            string direction=Term::toString(direct);//Convertir les directions en forme de direction en forme de string
            if(direction=="In")//Si le direction est In
            {
                const QPointF points[5]=
                {
                    QPointF(xToScreenX(termpoint.getX()),yToScreenY(termpoint.getY())),
                    QPointF(xToScreenX(termpoint.getX()-5.0),yToScreenY(termpoint.getY()+5.0)),
                    QPointF(xToScreenX(termpoint.getX()-15.0),yToScreenY(termpoint.getY()+5.0)),
                    QPointF(xToScreenX(termpoint.getX()-15.0),yToScreenY(termpoint.getY()-5.0)),
                    QPointF(xToScreenX(termpoint.getX()-5.0),yToScreenY(termpoint.getY()-5.0))

                };//Intialiser les 5 points pour dessiner le polygone de tag d'entree
                painter.setPen ( QPen ( Qt::yellow , 2 ) );//Set pen en jaune
                painter.setBrush ( QBrush ( Qt::darkGray ) );//Set brush en gris
                painter.drawPolygon(points,5);//draw polygon nous permet de dessiner Term (a,b)
            }

            if(direction=="Out")//Si le direction est Out
            {
                const QPointF points[5]=
                {
                    QPointF(xToScreenX(termpoint.getX()),yToScreenY(termpoint.getY())),
                    QPointF(xToScreenX(termpoint.getX()+5.0),yToScreenY(termpoint.getY()-5.0)),
                    QPointF(xToScreenX(termpoint.getX()+15.0),yToScreenY(termpoint.getY()-5.0)),
                    QPointF(xToScreenX(termpoint.getX()+15.0),yToScreenY(termpoint.getY()+5.0)),
                    QPointF(xToScreenX(termpoint.getX()+5.0),yToScreenY(termpoint.getY()+5.0))

                };//Intialiser les 5 points pour dessiner le polygone de tag de sortie
                painter.setPen ( QPen ( Qt::yellow , 2 ) );
                painter.setBrush ( QBrush ( Qt::darkGray  ));
                painter.drawPolygon(points,5);//draw polygon nous permet de dessiner Term (sout,cout)
            }

        }

        for ( size_t i=0 ; i < instances.size() ; ++i ) //parcourir les instances
        {
            //cerr<<i<<endl;
            Point instPos = instances[i] -> getPosition ();//Prendre les positions d'instances pour translater

            QString nameinstance=instances[i]->getName().c_str();//Trouver les noms d'instances en forme de QString
            QFont font=painter.font();
            font.setPixelSize(13);
            painter.setPen ( QPen ( Qt::darkGreen , 1 ) );
            painter.setFont(font);
            painter.drawText(xToScreenX(instPos.getX()+10.0),yToScreenY(instPos.getY()-15),nameinstance);//Dessiner les noms par la methode de drawText



            const Symbol* symbol = instances[i] -> getMasterCell() -> getSymbol();//Entrer le symbol pour trouver les shapes dans le symbol
            if ( not symbol ) continue ;
            //if ( flags & InstanceShapes ) {
            const vector<Shape*>& shapes=symbol->getShapes(); //dans instances, on aura les shapes qu'on va tracer

            for ( size_t j =0 ; j < shapes.size() ; ++j )
            {
                //cout<<"test"<<endl;
                BoxShape * boxShape = dynamic_cast < BoxShape* >( shapes[j]);//Recupere les boxshapes
                if ( boxShape )
                {
                    Box box = boxShape -> getBoundingBox();//Creer et initialiser la boxe par getBoundingBox
                    painter.setPen ( QPen ( Qt::red , 1 ) );
                    painter.setBrush ( QBrush ( Qt::black ) );
                    QRect rect = boxToScreenRect ( box.translate ( instPos ));//Convertir et translater la boxe
                    painter.drawRect ( rect ); //tracer une boite rouge, comme remplissage noir (exemple halfadder)
                }

                LineShape* lineShape = dynamic_cast < LineShape * >( shapes[j]);//Recupere les lignes
                if ( lineShape )
                {

                    painter.setPen ( QPen ( Qt::green , 1 ) );
                    Point p0=Point(lineShape->getX1(),lineShape->getY1());//Recupere la point de start
                    Point p1=Point(lineShape->getX2(),lineShape->getY2());//Recupere la point de end
                    p0=p0.translate(instPos);
                    p1=p1.translate(instPos);//Translate les deux points par instPos
                    QLine line=QLine(xToScreenX(p0.getX()),yToScreenY(p0.getY()),xToScreenX(p1.getX()),yToScreenY(p1.getY()));//Creer QLine pour dessiner
                    painter.drawLine ( line ); //tracer les lines (exemple line en vert pour les portes logiques)
                }

                EllipseShape* ellipseShape = dynamic_cast < EllipseShape * >( shapes[j]);//Recupere les ellipses
                if ( ellipseShape )
                {
                    //<<"test_ellipse"<<endl;
                    Box box = ellipseShape -> getBoundingBox();//Les ellipses sont en forme de boxes
                    painter.setPen ( QPen ( Qt::white , 1 ) );
                    painter.setBrush ( QBrush ( Qt::black ) );
                    QRect ellipse = boxToScreenRect ( box.translate ( instPos ));
                    painter.drawEllipse ( ellipse ); //tracer ellipse pour le PMOS 
                }

                ArcShape* arcShape = dynamic_cast < ArcShape * >( shapes[j]);//Recupere les arcs
                if ( arcShape )
                {
                    //cout<<"test_arc"<<endl;
                    Box box = arcShape -> getBoundingBox();//les arcs sont limités par la forme de box
                    painter.setPen ( QPen ( Qt::green , 1 ) );
                    QRect arc = boxToScreenRect ( box.translate ( instPos ));
                    painter.drawArc ( arc, (arcShape->getStart())*16 ,(arcShape->getSpan())*16 ); 
                    //tracer la porte logique or, xor. Les angles de start et span doivent être multipliés par 16 pour trouver les bonnes formes
                }

                TermShape* termShape = dynamic_cast < TermShape * >( shapes[j]);//Recupere les terms
                if ( termShape )
                {
                    //cout<<"test_term"<<endl;
                    Box box = termShape -> getBoundingBox();
                    //cerr<<"x1:"<<box.getX1()<<endl;
                    //cerr<<"x2:"<<box.getX2()<<endl;
                    box=box.translate(-5,5);
                    box=box.inflate(-3); //on a reduit la taille pour un meilleur effet visuel
                    painter.setPen ( QPen ( Qt::red , 1 ) );
                    painter.setBrush ( QBrush ( Qt::red ) );
                    QRect rect = boxToScreenRect ( box.translate ( instPos ));
                    painter.drawRect ( rect ); //tracer des terms des portes logiques, PMOS, NOS, GND, Vdd

                    QString name=termShape->getTerm()->getName().c_str();
                    //cerr<<name.toStdString ()<<endl;
                    //Point termpoint=termShape->getTerm()->getPosition();
                    //cerr<<"x:"<<termpoint.getX()<<endl;
                    //cerr<<"y:"<<termpoint.getY()<<endl;

                    QFont font=painter.font();
                    font.setPixelSize(10);
                    painter.setPen ( QPen ( Qt::darkYellow , 1 ) );
                    painter.setFont(font);
                    TermShape::NameAlign a= termShape->getAlign();
                    string align = TermShape::toString(a);
                    //Et ensuite afficher leur nom a coté (en darkYellow), par rapport à leur allignement
                    if(align=="top_left")
                    {
                        painter.drawText(xToScreenX(box.getX1()-10),yToScreenY(box.getY2()+10),name);
                    }

                    if(align=="top_right")
                    {
                        painter.drawText(xToScreenX(box.getX2()+10),yToScreenY(box.getY2()+10),name);
                    }

                    if(align=="bottom_left")
                    {
                        painter.drawText(xToScreenX(box.getX1()-10),yToScreenY(box.getY1()-10),name);
                    }

                    if(align=="bottom_right")
                    {
                        painter.drawText(xToScreenX(box.getX2()+10),yToScreenY(box.getY1()-10),name);
                    }//Les 4 if pour bien poser les 4 positions 
                }




            }
            //}
        }
    }

    else//Si il existe pas les instances, donc on doit dessiner par cell directement
    {

        QString namecell=cell_->getName().c_str();//Recupere les noms de cell
        QFont font=painter.font();
        font.setPixelSize(13);
        painter.setPen ( QPen ( Qt::darkGreen , 1 ) );
        painter.setFont(font);
        painter.drawText(xToScreenX(-10),yToScreenY(-20),namecell);//Dessiner les noms des cells







        for ( size_t i=0 ; i < shapes.size() ; ++i )//Iterateur de shapes pour trouver les different shapes
        {
            BoxShape * boxShape = dynamic_cast < BoxShape* >( shapes[i]);
            if ( boxShape )
            {
                Box box = boxShape -> getBoundingBox();
                painter.setPen ( QPen ( Qt::red , 1 ) );
                //painter.setBrush ( QBrush ( Qt::red ) );
                painter.setBrush ( QBrush ( Qt::black ) );
                QRect rect = boxToScreenRect ( box);
                painter.drawRect ( rect );//Meme dans la partie d'instance
            }

            LineShape* lineShape = dynamic_cast < LineShape * >( shapes[i]);
            if ( lineShape )
            {

                painter.setPen ( QPen ( Qt::green , 1 ) );
                Point p0=Point(lineShape->getX1(),lineShape->getY1());
                Point p1=Point(lineShape->getX2(),lineShape->getY2());
                QLine line=QLine(xToScreenX(p0.getX()),yToScreenY(p0.getY()),xToScreenX(p1.getX()),yToScreenY(p1.getY()));
                painter.drawLine ( line );
            }

            EllipseShape* ellipseShape = dynamic_cast < EllipseShape * >( shapes[i]);
            if ( ellipseShape )
            {
                //cout<<"test_ellipse"<<endl;
                Box box = ellipseShape -> getBoundingBox();
                painter.setPen ( QPen ( Qt::white , 1 ) );
                painter.setBrush ( QBrush ( Qt::black ) );
                QRect ellipse = boxToScreenRect ( box);
                painter.drawEllipse ( ellipse );
            }

            ArcShape* arcShape = dynamic_cast < ArcShape * >( shapes[i]);
            if ( arcShape )
            {
                //cout<<"test_arc"<<endl;
                Box box = arcShape -> getBoundingBox();
                painter.setPen ( QPen ( Qt::green , 1 ) );
                QRect arc = boxToScreenRect ( box);
                painter.drawArc ( arc, (arcShape->getStart())*16 ,(arcShape->getSpan())*16 );
            }

            TermShape* termShape = dynamic_cast < TermShape * >( shapes[i]);
            if ( termShape )
            {
                //cout<<"test_term"<<endl;
                Box box = termShape -> getBoundingBox();
                //cerr<<"x1:"<<box.getX1()<<endl;
                //cerr<<"x2:"<<box.getX2()<<endl;
                box=box.translate(-5,5);
                box=box.inflate(-3);
                painter.setPen ( QPen ( Qt::red , 1 ) );
                painter.setBrush ( QBrush ( Qt::red ) );
                QRect rect = boxToScreenRect ( box);
                painter.drawRect ( rect );

                QString name=termShape->getTerm()->getName().c_str();
                //cerr<<name.toStdString ()<<endl;
                //Point termpoint=termShape->getTerm()->getPosition();
                //cerr<<"x:"<<termpoint.getX()<<endl;
                //cerr<<"y:"<<termpoint.getY()<<endl;

                QFont font=painter.font();
                font.setPixelSize(10);
                painter.setPen ( QPen ( Qt::red , 1 ) );
                painter.setFont(font);
                TermShape::NameAlign a= termShape->getAlign();
                string align = TermShape::toString(a);
                if(align=="top_left")
                {
                    painter.drawText(xToScreenX(box.getX1()-10),yToScreenY(box.getY2()+10),name);
                }

                if(align=="top_right")
                {
                    painter.drawText(xToScreenX(box.getX2()+10),yToScreenY(box.getY2()+10),name);
                }

                if(align=="bottom_left")
                {
                    painter.drawText(xToScreenX(box.getX1()-10),yToScreenY(box.getY1()-10),name);
                }

                if(align=="bottom_right")
                {
                    painter.drawText(xToScreenX(box.getX2()+10),yToScreenY(box.getY1()-10),name);
                }
            }
        }
    }

    const vector < Net* > &nets = cell_ -> getNets();

    for ( size_t i=0 ; i < nets.size() ; ++i ) //dans cette partie du code, on va parcourir nets
    {
        const vector < Line* > &lines = nets[i]->getLines(); //pour ce faire, on recupere les lines
        //cerr << "net :" << nets[i]->getName() << endl;

        for ( size_t j=0 ; j < lines.size() ; ++j )
        {
            Point source=lines[j]->getSourcePosition(); //ensuite on aura une source
            Point target=lines[j]->getTargetPosition(); //et une cible
            //cerr<<"source:"<<source.getX()<<" "<<source.getY()<<endl;
            //cerr<<"target:"<<target.getX()<<" "<<target.getY()<<endl;
            painter.setPen ( QPen ( Qt::cyan , 1 ) );

            QLine line1=QLine(xToScreenX(source.getX()),yToScreenY(source.getY()),xToScreenX(target.getX()),yToScreenY(target.getY()));
            painter.drawLine ( line1 ); //ligne en cyan qui connecte 2 points(source et target)




        }

        const vector < Node* > &nodes = nets[i]->getNodes();
        for( size_t j=0 ; j < nodes.size() ; ++j ) //detection des nodes, pour ensuite afficher les noeuds (blanc,magenta)
        {

            NodePoint* npoint= dynamic_cast<NodePoint*>(nodes[j]); //variable pour tester si source/target correspond
            if(npoint)
            {
                size_t cmpt=0;
                const vector < Line* > &lines = npoint->getNet()->getLines();
                for( size_t k=0 ; k < lines.size() ; ++k )
                {


                    Node* source=lines[k]->getSource();
                    Node* target=lines[k]->getTarget();
                    if(npoint==source or npoint==target)
                    {
                        cmpt++; //si c'est le cas, on incrémente le compteur
                    }
                    if(cmpt>2) //une fois que le compteur est supérieur à 2, on sait que les 3 lignes sont connectées
                    {
                        painter.setPen ( QPen ( Qt::white , 1 ) );
                        painter.setBrush ( QBrush ( Qt::magenta ) );
                        Box elli=Box(npoint->getPosition().getX()-3,npoint->getPosition().getY()-3,npoint->getPosition().getX()+3,npoint->getPosition().getY()+3);
                        QRect Ellip=boxToScreenRect(elli);
                        painter.drawEllipse ( Ellip ); //donc on va tracer notre noeud sur la figure
                        //cmpt=0;
                        //break;
                    }


                }

            }

        }


    }



}



void CellWidget :: keyPressEvent ( QKeyEvent * event ) //gestion des touches clavier( up_key, down_key ...)
{
    event -> ignore ();
    if ( event -> modifiers() & ( Qt :: ControlModifier | Qt :: ShiftModifier ))
        return ;
    switch ( event -> key())
    {
    case Qt :: Key_Up :
        goUp();
        break ;
    case Qt :: Key_Down :
        goDown();
        break ;
    case Qt :: Key_Left :
        goLeft();
        break ;
    case Qt :: Key_Right :
        goRight();
        break ;
    default :
        return ;
    }
    event -> accept();
}


void CellWidget::goRight() //dépacement à droite
{
    viewport_.translate ( Point (20 ,0) );
    repaint();
}


void CellWidget::goLeft () //dépacement à gauche
{
    viewport_.translate ( Point (-20 ,0) );
    repaint();
}

void CellWidget::goUp () //dépacement en haut
{
    viewport_.translate ( Point (0 ,20) );
    repaint();
}

void CellWidget::goDown() //dépacement en bas
{
    viewport_.translate ( Point (0 ,-20) );
    repaint();
}

}  // Netlist namespace.
