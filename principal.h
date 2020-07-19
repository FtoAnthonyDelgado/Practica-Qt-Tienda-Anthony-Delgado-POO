#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include <dialogo.h>
#include <dialog2.h>


#include "producto.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Principal; }
QT_END_NAMESPACE

class Principal : public QMainWindow
{
    Q_OBJECT

public:
    Principal(QWidget *parent = nullptr);
    ~Principal();

public slots:
    void mostrarPrecio(int);
    void agregarProducto();

private slots:
    void on_ventana1_triggered();

    void on_actionV_er_productos_triggered();

    void on_ventana2_2_triggered();

    void on_ventana2_triggered();

private:
    Ui::Principal *ui;
    void limpiar();
    QList<Producto*> m_productos;

    void inicializarDatos();
    void inicializarWidgets();
    float m_subtotal;

    void calcular(float subtotal);
    bool buscar(Producto *producto, int cantidad);

};
#endif // PRINCIPAL_H
