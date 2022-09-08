#include <QtWidgets/QApplication>
#include "Controller.h"
#include "FileRepo.h"
#include "TenantsGUI.h"
#include "Tests.h"

int main(int argc, char* argv[])
{
    // Run every test
    test_everything();

    QApplication a(argc, argv);
    
    // Use the 30.000 lines file
    std::shared_ptr<FileRepo> repository{ new FileRepo{ "tenants_big.txt" } };

    Controller service{ repository };
    TenantsGUI w{ service };
    w.show();

    return a.exec();
}
