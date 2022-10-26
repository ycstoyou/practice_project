#include "practice_project.h"
#include "opendicom.h"
#include <qdebug.h>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmfg/fginterface.h"
#include "dcmtk/dcmfg/fgbase.h"
#include "dcmtk/dcmfg/fgplanpo.h"
#include "dcmtk/dcmjpeg/djencode.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include < QImage>
using namespace std;
practice_project::practice_project(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	opendicom dicomItem;
	InitView();
	//dicomItem.open_dicom_files();

}

practice_project::~practice_project()
{}
unsigned long LoadFlags(void)
{
	const unsigned long flags =
		CIF_UsePartialAccessToPixelData |
		CIF_AcrNemaCompatibility |
		0;

	return flags;
}

void practice_project::InitView()
{
	DcmFileFormat* dfile = new DcmFileFormat();
	OFCondition cond = dfile->loadFile("FILE229.dcm", EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength);

	if (cond.good())
	{
		E_TransferSyntax xfer = dfile->getDataset()->getOriginalXfer();
		auto dataset = dfile->getDataset();

		DicomImage img(dataset, dataset->getOriginalXfer(), LoadFlags(), 0, 1);

		image_data = new byte[(512 * 512) * 4];

		auto di_inter = img.getInterData();
		src = (short*)di_inter->getData();

		image_src = new short[512 * 512];

		memcpy(image_src, src, 512 * 512 * 2);

		w = (static_cast<int>(img.getWidth()));
		h = (static_cast<int>(img.getHeight()));

		OFString window_center, window_width;
		dfile->getDataset()->findAndGetOFString(DCM_WindowCenter, window_center);
		dfile->getDataset()->findAndGetOFString(DCM_WindowWidth, window_width);

		nCenter = atoi(window_center.data());
		nWidth = atoi(window_width.data());

		windowing(nWidth, nCenter, image_src, image_data, w, h);

		thumbnail_image = QImage(image_data, w, h, QImage::Format::Format_RGB32);

		label = new QLabel;
		label->setPixmap(QPixmap::fromImage(thumbnail_image));
		label->show();
		QHBoxLayout* mainlayout = new QHBoxLayout();
		QFrame* mainframe = new QFrame();

		UpdateWindowTitle();
		mainlayout->addWidget(label);
		mainframe->setLayout(mainlayout);

		setLayout(mainlayout);

	}
}


void practice_project::UpdateAdjustWindowin(int newCenter)
{
	nCenter = nCenter + newCenter;

	windowing(nWidth, nCenter, image_src, image_data, w, h);
	thumbnail_image = QImage(image_data, w, h, QImage::Format::Format_RGB32);

	label->setPixmap(QPixmap::fromImage(thumbnail_image));
	UpdateWindowTitle();
}

void practice_project::windowing(int ww, int wc, short* pSrc, byte* pDst, int width, int height)
{
	int min_val = wc - ww / 2;

	for (int i = 0; i < width * height; i++)
	{
		// normalize [0,1]
		float val = (float)(pSrc[i] - min_val) / ww;
		val *= 255;
		byte pixel_value;

		if (val < 0)
			pixel_value = 0;
		else if (val > 255)
			pixel_value = 255;
		else
			// + 0.5 for round off
			pixel_value = (int)(val + 0.5f);

		pDst[i * 4 + 0] = pixel_value;
		pDst[i * 4 + 1] = pixel_value;
		pDst[i * 4 + 2] = pixel_value;
		pDst[i * 4 + 3] = pixel_value;
		//i = i + 3;
	}
	return;

}

void practice_project::keyPressEvent(QKeyEvent* event)

{
	int key = event->key();
	switch (key)
	{
	case Qt::Key_Up:

		qDebug() << event->key();

		UpdateAdjustWindowin(100);
		//qDebug() << __FUNCTION__ << "multi otf mode :" << render_view()->enable_multi_otf();
		break;
	case Qt::Key_Down:
		qDebug() << event->key();
		//ResetCamera();
		UpdateAdjustWindowin(-100);
		break;
	default:
		break;
	}

}

void practice_project::UpdateWindowTitle()
{
	QString windowing;
	windowing = QString("width %1 center %2").arg(QString::number(nWidth)).arg(QString::number(nCenter));

	setWindowTitle(windowing);
}

