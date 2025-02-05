﻿#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DeviceAddTable/DeviceAddModel.h"
#include <QTextCodec>
#include "DeviceAddTable/PatientCaseModel.h"
#include <qqml.h>
#include "zmqcontext.h"
#include "zmqsocket.h"
#include "bytearraytools.h"
#include "DeviceAddTable/PatientCaseListModel.h"
#include "Patient/ShowImage.h"
#include "DeviceAddTable/PatientPresAddModel.h"
#include "DeviceAddTable/ChoiseCaseModel.h" //管理界面
#include "DeviceAddTable/TreatCaseModel.h"  //管理界面 处方管理
#include "DeviceAddTable/GLPatientManagerModel.h" //管理界面 患者管理
#include "DeviceAddTable/DoctorManagerModel.h" //管理界面 用户管理
#include "DeviceAddTable/PatientCaseListNewModel.h" //添加处方界面
#include "DeviceAddTable/TreatCaseManageListModel.h"
#include "DeviceAddTable/PatientTreatmentRecordModel.h"
#include "DeviceAddTable/ChoiseCasePlanListModel.h"

static QObject *byteArrayTools_factory(QQmlEngine *, QJSEngine *)
{
    return new ByteArrayTools;
}

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QApplication app(argc, argv);
    qmlRegisterType<ZMQSocket>("zmq.Components", 1, 0, "ZMQSocket");
    qmlRegisterSingletonType<ByteArrayTools>("zmq.Components", 1, 0, "BAT", byteArrayTools_factory);

    QQmlApplicationEngine engine;
    engine.addImportPath(TaoQuickImportPath);
    engine.rootContext()->setContextProperty("taoQuickImagePath", TaoQuickImagePath);

    engine.rootContext()->setContextProperty("mainImageResoucePath",MainImageResourcePath);
    DeviceAddModel model;
    engine.rootContext()->setContextProperty("deviceAddModel", &model); //医生界面 病人列表

    PatientTreatmentRecordModel patientTreatmentRecordModel;
    engine.rootContext()->setContextProperty("patientTreatmentRecordModel",&patientTreatmentRecordModel);

    PatientCaseModel model1;
    engine.rootContext()->setContextProperty("patientCaseModel", &model1); //医生界面 病例

    ChoiseCasePlanListModel  model2;

    PatientCaseListModel treatManagerCheck;
    PatientCaseParaModel parm1;
    parm1.paraName="仰卧手向上";
    parm1.paraCase = "手少阴心经";
    treatManagerCheck.addModel(parm1);
    PatientCaseParaModel parm2;
    parm2.paraName="仰卧手向上";
    parm2.paraCase = "足厥阴肝经";



    treatManagerCheck.addModel(parm2);
    engine.rootContext()->setContextProperty("treatManagerCheckModel",&treatManagerCheck);  //开处方用的部分

    engine.rootContext()->setContextProperty("jingluoplanModel",&model2);  //开处方用的部分

    PatientCaseListNewModel newmodel;
    engine.rootContext()->setContextProperty("jingluoplannewModel",&newmodel);

    TreatCaseManageListModel treatcasemodel;
    engine.rootContext()->setContextProperty("treatcasemanageModel",&treatcasemodel);

    PatientPresAddModel addmodel;
    PatientCasePresParaModel aparam1;
    aparam1.paraId="1111";
    aparam1.paraDoctor="王三";
    aparam1.paraScript="你好吗";
    aparam1.paraTime="20210714";
    addmodel.addModel(aparam1);
    addmodel.addModel(aparam1);
    engine.rootContext()->setContextProperty("casedatalistModel",&addmodel);


    GLPatientManagerModel glModel;
     engine.rootContext()->setContextProperty("glpatientModel", &glModel);
    DoctorManagerModel gldoctorModel;
     engine.rootContext()->setContextProperty("gldoctorModel", &gldoctorModel);

    TreatCaseModel caseModel;
    engine.rootContext()->setContextProperty("treatcaseModel",&caseModel);

    ChoiseCaseModel choisemodel;
    engine.rootContext()->setContextProperty("choisecaseModel",&choisemodel);
    ShowImage *CodeImage = new ShowImage();
    engine.rootContext()->setContextProperty("CodeImage",CodeImage);
    engine.addImageProvider(QLatin1String("CodeImg"), CodeImage->m_pImgProvider);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);


    return app.exec();
}
