#include "principal.h"
#include "ui_principal.h"
#include "producto.h"



#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    inicializarDatos();
    m_subtotal = 0;
}

Principal::~Principal()
{
    delete ui;
}

void Principal::mostrarPrecio(int index)
{
    // Obtener el precio del producto seleccionado
    float precio = m_productos.at(index)->precio();

    // Colacar el precio en la etiqueta correspondiente
    ui->outPrecio->setText("$ " + QString::number(precio));
}

void Principal::agregarProducto()
{
    // Obteniendo datos desde la interfaz
    int index = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(index);
    int cantidad = ui->inCantidad->value();

    if (cantidad == 0){
        return;
    }

    // Calcular subtotal
    float subtotal = cantidad * p->precio();


    //Buscar y actualizar productos repetidos
    if (!buscar(p, cantidad)) {

    // Agregar datos a la tabla
    int posicion = ui->outDetalle->rowCount();
    ui->outDetalle->insertRow(posicion);
    ui->outDetalle->setItem(posicion,0,new QTableWidgetItem(QString::number(cantidad)));
    ui->outDetalle->setItem(posicion,1,new QTableWidgetItem(p->nombre()));
    ui->outDetalle->setItem(posicion,2,new QTableWidgetItem(QString::number(subtotal)));
   }

    // Limpiar datos
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();


    // Calcular valores y mostrar
    calcular(subtotal);

}

void Principal::inicializarDatos()
{
    //abrir un archivo de productos
    QFile archivo("producto.csv");

    //leer el archivo de productos
    if(archivo.open(QFile::ReadOnly)) {
        QTextStream in(&archivo);
        while (!in.atEnd()){
            QString linea = in.readLine();
            // separar datos por ';'
            QStringList datos = linea.split(';');
            int codigo = datos[0].toInt();
            float precio = datos[2].toFloat();
            m_productos.append(new Producto(codigo,datos[1],precio));

        }
}else{
        qDebug() <<"Error al abrir el archivo";

}
    archivo.close();


    // Crear y agregar productos a la lista
    m_productos.append(new Producto(1,"Leche",0.75));
    m_productos.append(new Producto(2,"Pan",0.15));
    m_productos.append(new Producto(3,"Huevos",0.15));
    m_productos.append(new Producto(4,"Azucar",0.60));
    m_productos.append(new Producto(5,"Cafe 400g",8.5));
    m_productos.append(new Producto(6,"Arroz",0.90));
    m_productos.append(new Producto(7,"Paquete de salchichas",0.85));
    m_productos.append(new Producto(8,"Fideos",1.25));






    // Invocar al metodo para inicializar los widgets
    inicializarWidgets();
}

void Principal::inicializarWidgets()
{
    // Agregar nombre de los productos al combo box
    for (int i = 0; i < m_productos.length(); ++i) {
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    }

    // Colocar cabecera en la tabla
    QStringList cabecera = {"Cantidad", "Producto", "Sub Total"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);

    // Eventos
    connect(ui->inProducto, SIGNAL(currentIndexChanged(int)), this, SLOT(mostrarPrecio(int)));
    connect(ui->cmdAgregar, SIGNAL(clicked(bool)), this, SLOT(agregarProducto()));

    // Mostrar el precio del primer producto
    mostrarPrecio(0);
}

void Principal::calcular(float subtotal)
{
    m_subtotal += subtotal;
    float iva = m_subtotal * 0.12;
    float total = m_subtotal + iva;

    ui->outSubtotal->setText(QString::number(m_subtotal));
    ui->outIva->setText(QString::number(iva));
    ui->outTotal->setText(QString::number(total));


}

bool Principal::buscar(Producto *producto, int cantidad)
{
    //Obtener el numero de filas
    int numFilas = ui->outDetalle->rowCount();
    //Recorrer la tabla
    for(int i =0; i < numFilas; ++i) {
        // Obtener el item en la fila y la columna 1 (nombre del producto)
        QTableWidgetItem *item = ui->outDetalle->item(i,1);
        //Obtener el texto
        QVariant dato = item->data(Qt::DisplayRole).toString();
        //comparar producto
        if (dato == producto->nombre()){
            //obtener la cantidad del prodecto que esta en el detalle
            int cantidadActual = item->data(Qt::DisplayRole).toInt();
            //sumar la cantidad
            int cantidadNueva = cantidadActual + cantidad;
            //calcular nuevo sub total
            float subtotal = cantidadNueva * producto->precio();
            //Actualizar en la tabla
            ui->outDetalle->setItem(i,0,new QTableWidgetItem(QString::number(cantidadNueva)));
            ui->outDetalle->setItem(i,2,new QTableWidgetItem(QString::number(subtotal)));
            return true;


        }

    }
    return false;
}


void Principal::on_ventana1_triggered()
{
    Dialogo *ventana1 = new Dialogo;
    ventana1->setModal(true);
    ventana1->show();
}


void Principal::on_ventana2_triggered()
{
    Dialog2 *ventana2 = new Dialog2(this);
    ventana2->setVisible(true);
}
