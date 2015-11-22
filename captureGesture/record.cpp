//OpenCV Headers
#include <opencv/cv.h>
#include <opencv/highgui.h>
//Other headers
#include <stdio.h>
#include <vector>
#include <string>
#include <cvblob.h>
#include <string.h>
#include <errno.h>

using namespace cv;
using namespace cvb;

int main(int argc, char const *argv[])
{
	CvCapture* cap=cvCreateCameraCapture(0);
	IplImage* mframe;//, seg_a, seg_b, seg_c, seg_d, seg_e, labelImg;
	// IplImage* yuv_a, yuv_b, yuv_c, yuv_d, yuv_e;
	IplImage* frame = cvCreateImage(cvSize(640, 400), 8, 3);
	cvNamedWindow("LiveFeed",CV_WINDOW_AUTOSIZE);
	int gest_no = 0;
	cvNamedWindow("sliders", CV_WINDOW_NORMAL);
	cvCreateTrackbar("gesture number","sliders",&gest_no,5,0);
	vector<string> gestureName;
	gestureName.push_back("circle");
	gestureName.push_back("leftright");
	gestureName.push_back("updown");
	vector<int> gestCount(5, 0);

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
	bool record = false;
	int time_steps = 35;
	int time_rem = 0;
	vector<double> Ax;
	vector<double> Bx;
	vector<double> Cx;
	vector<double> Dx;
	vector<double> Ex;
	vector<double> Ay;
	vector<double> By;
	vector<double> Cy;
	vector<double> Dy;
	vector<double> Ey;
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
	    	if(time_rem == 0)
	    	{
	    		record = false;
	    		gestCount[gest_no]++;
	    		char t_path[100];
	    		char file_path[100];
	    		strcpy(t_path, "./dataset/");
	    		strcat(t_path, gestureName[gest_no].c_str());
	    		sprintf(file_path, "%s/%s-%d.tgd", t_path, gestureName[gest_no].c_str(), gestCount[gest_no]);
	    		FILE* fp = fopen(file_path, "w");
	    		for (int i = 0; i < Ax.size(); ++i)
	    		{
	    			fprintf(fp, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", Ax[i], Bx[i], Cx[i], Dx[i], Ex[i], Ay[i], By[i], Cy[i], Dy[i], Ey[i]);
	    		}
	    		fclose(fp);
	    		printf("Data recorded at %s\n", file_path);
	    	}
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
			time_rem--;
	    }
	    //Escape Sequence
	    char c = cvWaitKey(1);
	    //If the key pressed by user is Esc(ASCII is 27) then break out of the loop
	    if(c==27)
	       break;
	   	if(c == 'd' || c == 'D')
	   	{
	   		record = true;
	   		time_rem = time_steps;
	   		Ax.clear();
	   		Bx.clear();
	   		Cx.clear();
	   		Dx.clear();
	   		Ex.clear();
	   		Ay.clear();
	   		By.clear();
	   		Cy.clear();
	   		Dy.clear();
	   		Ey.clear();
	   	}
	}
	return 0;
}