#pragma once

#include <QtWidgets/QDialog>
#include "ui_practice_project.h"
#include <QtWidgets>
class practice_project : public QDialog
{
    Q_OBJECT

public:
    practice_project(QWidget *parent = nullptr);
    ~practice_project();

    void InitView();
	void windowing(int ww, int wc,  short* pSrc, byte* pDst, int width, int height);
	void AdjustWindowing(int ww, int wc, byte* pSrc, byte* pDst, int width, int height);
	int GetWindowingVoxcelColor(int min_ww, int max_ww, int wc, int current_voxcel);

	virtual void  keyPressEvent(QKeyEvent* event);
	void UpdateWindowTitle();
	void UpdateAdjustWindowin(int newCenter);
	static short bytesToShort(byte* Value);
public:
	int nCenter =0;
	int nWidth =0;

	QImage thumbnail_image;
	byte* image_data;
	short *image_src;
	short* src;
	int w;
	int h;
	QLabel* label;
private:
    Ui::practice_projectClass ui;
};

