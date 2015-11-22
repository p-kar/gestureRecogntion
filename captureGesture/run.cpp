#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "matrix.h"
//OpenCV Headers
#include <opencv/cv.h>
#include <opencv/highgui.h>
//Other headers
#include <vector>
#include <deque>
#include <string>
#include <cvblob.h>
#include <errno.h>

#define  BUFSIZE 256

using namespace cv;
using namespace cvb;
using namespace std;

void getPredictionMATLAB(Engine* ep, deque<double> &Ax, deque<double> &Bx, deque<double> &Cx, deque<double> &Dx, deque<double> &Ex, deque<double> &Ay, deque<double> &By, deque<double> &Cy, deque<double> &Dy, deque<double> &Ey)
{
	if(Ax.size() < 35)
		return;
	mxArray *T = NULL, *result = NULL;
	double datapoint[Ax.size()][10];
	T = mxCreateDoubleMatrix(Ax.size(), 10, mxREAL);
	for (int i = 0; i < Ax.size(); ++i)
	{
		datapoint[i][0] = Ax[i];
		datapoint[i][1] = Bx[i];
		datapoint[i][2] = Cx[i];
		datapoint[i][3] = Dx[i];
		datapoint[i][4] = Ex[i];
		datapoint[i][5] = Ay[i];
		datapoint[i][6] = By[i];
		datapoint[i][7] = Cy[i];
		datapoint[i][8] = Dy[i];
		datapoint[i][9] = Ey[i];
	}
	memcpy((void *)mxGetPr(T), (void *)datapoint, sizeof(datapoint));
	engEvalString(ep, "clear;");
	engPutVariable(ep, "T", T);
	engEvalString(ep, "pred = printPrediction(T);");
	result = engGetVariable(ep,"pred");
	char* pred = (char*) malloc(BUFSIZE);
	mxGetString(result, pred, BUFSIZE);
	printf("Predicted gesture: %s\n", pred);
	mxDestroyArray(T);
	mxDestroyArray(result);
}

int main(int argc, char const *argv[])
{
	Engine *ep;
	if (!(ep = engOpen(""))) {
		fprintf(stderr, "\nCan't start MATLAB engine\n");
		return EXIT_FAILURE;
	}
	CvCapture* cap=cvCreateCameraCapture(0);
	IplImage* mframe;
	IplImage* frame = cvCreateImage(cvSize(640, 400), 8, 3);
	cvNamedWindow("LiveFeed",CV_WINDOW_AUTOSIZE);

	IplImage* labelImg = cvCreateImage(cvSize(640, 480),IPL_DEPTH_LABEL,1);
	IplImage* seg_a = cvCreateImage(cvSize(640, 400), 8, 1);
	IplImage* seg_b = cvCreateImage(cvSize(640, 400), 8, 1);
	IplImage* seg_c = cvCreateImage(cvSize(640, 400), 8, 1);
	IplImage* seg_d = cvCreateImage(cvSize(640, 400), 8, 1);
	IplImage* seg_e = cvCreateImage(cvSize(640, 400), 8, 1);
	IplImage* yuv_a = cvCreateImage(cvSize(640, 400), 8, 3);
	IplImage* yuv_b = cvCreateImage(cvSize(640, 400), 8, 3);
	IplImage* yuv_c = cvCreateImage(cvSize(640, 400), 8, 3);
	IplImage* yuv_d = cvCreateImage(cvSize(640, 400), 8, 3);
	IplImage* yuv_e = cvCreateImage(cvSize(640, 400), 8, 3);

	CvBlobs blob_a, blob_b, blob_c, blob_d, blob_e;
	bool record = true;
	int time_steps = 35;
	deque<double> Ax;
	deque<double> Bx;
	deque<double> Cx;
	deque<double> Dx;
	deque<double> Ex;
	deque<double> Ay;
	deque<double> By;
	deque<double> Cy;
	deque<double> Dy;
	deque<double> Ey;
	double ax, bx, cx, dx, ex, ay, by, cy, dy, ey;
	while(1)
	{
	    //Load the next frame
	    mframe = cvQueryFrame(cap);
	    cvResize(mframe, frame);
	    cvCvtColor(frame,yuv_e,CV_BGR2YUV);
	    cvCvtColor(frame,yuv_b,CV_BGR2YUV);
	    cvCvtColor(frame,yuv_c,CV_BGR2YUV);
	    cvCvtColor(frame,yuv_a,CV_BGR2YUV);
	    cvCvtColor(frame,yuv_d,CV_BGR2YUV);
	    cvInRangeS(yuv_e, cvScalar(113, 110, 54), cvScalar(177, 177, 101), seg_e);
	    cvInRangeS(yuv_b, cvScalar(0, 71, 122), cvScalar(107, 118, 162), seg_b);
	    cvInRangeS(yuv_c, cvScalar(40, 99, 84), cvScalar(118, 140, 115), seg_c);
	    cvInRangeS(yuv_a, cvScalar(134, 156, 82), cvScalar(183, 255, 154), seg_a);
	    cvInRangeS(yuv_d, cvScalar(172, 112, 31), cvScalar(236, 152, 101), seg_d);
	    cvSmooth(seg_a,seg_a,CV_MEDIAN,7,7);
	    cvSmooth(seg_b,seg_b,CV_MEDIAN,7,7);
	    cvSmooth(seg_c,seg_c,CV_MEDIAN,7,7);
	    cvSmooth(seg_e,seg_e,CV_MEDIAN,7,7);
	    cvSmooth(seg_d,seg_d,CV_MEDIAN,7,7);
	    cvLabel(seg_a,labelImg,blob_a);
	    cvLabel(seg_b,labelImg,blob_b);
	    cvLabel(seg_c,labelImg,blob_c);
	    cvLabel(seg_d,labelImg,blob_d);
	    cvLabel(seg_e,labelImg,blob_e);
	    cvFilterByArea(blob_a,600,10000);
	    cvFilterByArea(blob_b,600,10000);
	    cvFilterByArea(blob_c,600,10000);
	    cvFilterByArea(blob_d,600,10000);
	    cvFilterByArea(blob_e,600,10000);
	    cvRenderBlobs(labelImg,blob_a,frame,frame);
	    cvRenderBlobs(labelImg,blob_b,frame,frame);
	    cvRenderBlobs(labelImg,blob_c,frame,frame);
	    cvRenderBlobs(labelImg,blob_d,frame,frame);
	    cvRenderBlobs(labelImg,blob_e,frame,frame);
	    //Show the present frame
	    cvShowImage("LiveFeed",frame);
	    if(record)
	    {
	    	for (CvBlobs::const_iterator it=blob_a.begin(); it!=blob_a.end(); ++it)
			{
				int xd, yd;
				xd =(it->second->maxx + it->second->minx )/2;
				yd =(it->second->maxy + it->second->miny)/2;
				xd = xd - 640/2;
				yd = -yd + 480/2;
				ax = ((double)xd)/640.0;
				ay = ((double)yd)/400.0;
				break;
			}
			for (CvBlobs::const_iterator it=blob_b.begin(); it!=blob_b.end(); ++it)
			{
				int xd, yd;
				xd =(it->second->maxx + it->second->minx )/2;
				yd =(it->second->maxy + it->second->miny)/2;
				xd = xd - 640/2;
				yd = -yd + 480/2;
				bx = ((double)xd)/640.0;
				by = ((double)yd)/400.0;
				break;
			}
			for (CvBlobs::const_iterator it=blob_c.begin(); it!=blob_c.end(); ++it)
			{
				int xd, yd;
				xd =(it->second->maxx + it->second->minx )/2;
				yd =(it->second->maxy + it->second->miny)/2;
				xd = xd - 640/2;
				yd = -yd + 480/2;
				cx = ((double)xd)/640.0;
				cy = ((double)yd)/400.0;
				break;
			}
			for (CvBlobs::const_iterator it=blob_d.begin(); it!=blob_d.end(); ++it)
			{
				int xd, yd;
				xd =(it->second->maxx + it->second->minx )/2;
				yd =(it->second->maxy + it->second->miny)/2;
				xd = xd - 640/2;
				yd = -yd + 480/2;
				dx = ((double)xd)/640.0;
				dy = ((double)yd)/400.0;
				break;
			}
			for (CvBlobs::const_iterator it=blob_e.begin(); it!=blob_e.end(); ++it)
			{
				int xd, yd;
				xd =(it->second->maxx + it->second->minx )/2;
				yd =(it->second->maxy + it->second->miny)/2;
				xd = xd - 640/2;
				yd = -yd + 480/2;
				ex = ((double)xd)/640.0;
				ey = ((double)yd)/400.0;
				break;
			}
			Ax.push_back(ax);
			Bx.push_back(bx);
			Cx.push_back(cx);
			Dx.push_back(dx);
			Ex.push_back(ex);
			Ay.push_back(ay);
			By.push_back(by);
			Cy.push_back(cy);
			Dy.push_back(dy);
			Ey.push_back(ey);
			if(Ax.size() > time_steps)
				Ax.pop_front();
			if(Bx.size() > time_steps)
				Bx.pop_front();
			if(Cx.size() > time_steps)
				Cx.pop_front();
			if(Dx.size() > time_steps)
				Dx.pop_front();
			if(Ex.size() > time_steps)
				Ex.pop_front();
			if(Ay.size() > time_steps)
				Ay.pop_front();
			if(By.size() > time_steps)
				By.pop_front();
			if(Cy.size() > time_steps)
				Cy.pop_front();
			if(Dy.size() > time_steps)
				Dy.pop_front();
			if(Ey.size() > time_steps)
				Ey.pop_front();
			getPredictionMATLAB(ep, Ax, Bx, Cx, Dx, Ex, Ay, By, Cy, Dy, Ey);
	    }
	    //Escape Sequence
	    char c = cvWaitKey(1);
	    //If the key pressed by user is Esc(ASCII is 27) then break out of the loop
	    if(c==27)
	       break;
	   	if(c == 'r' || c == 'R')
	   	{
	   		record = !record;
	   		if(record)
	   			printf("started recording\n");
	   		else
	   			printf("ended recording\n");
	   	}
	}
	engClose(ep);
	return 0;
}