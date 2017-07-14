//----------------------------------------------------------------------------
//���ߣ�Gingko
//������win10 64λ + vs2013 32λ + opencv2.4.13
//ʱ�䣺2017.07.14
//----------------------------------------------------------------------------
#include <opencv2\opencv.hpp>
#include <imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>

using namespace cv;
using namespace std;

Mat Diff_and_Detect(Mat video_t, Mat background)
{

	Mat foreground;
	Mat foreground_gray;
	Mat foreground_threshold;
	Mat foreground_x;

	vector<vector<Point>> contours_v;

	Rect rect;
	//���в�ֻ��ǰ��
	absdiff(video_t, background, foreground);

	//���ǰ���Ҷ�ͼ
	cvtColor(foreground, foreground_gray, CV_RGB2GRAY);

	//���ж�ֵ��
	threshold(foreground_gray, foreground_threshold, 35, 255, CV_THRESH_BINARY);

	//�����Ϸ��������������0���������������Ͻ�����������1ֵ�㣬���Ƕ���һ��δ�����bug����Э��
	//bug������������Ϸ����������ڶ�ֵ����ȫΪ0�������������Ʊ�Ե�����ڻ�����Ӿ���ʱ�����vector��Խ�硣��ʵ��ֻ�����Ϸ����������ֵ����ȫΪ0���Ͳ��ᴥ��bug��
	//����������£����½�������Ѿ�ע�͡���Ϊ���ڽ�ȡ�ı���backgroundʱ�����Ͻ������һ����0��0��0���ĵ㣬�����ӾͲ���������Ϸ���ֺ�ȫΪ0�������
	/*
	for (int i = 0; i < 60; i++)
	{
		for (int j = 15; j < 75; j++)
		{
			if (i == 0 && j == 74) continue;
			foreground_threshold.at<Vec2b>(i,j) = (0,0);
		}
	}
	*/

	cout << foreground_threshold.at<Vec2b>(0, 74)[0];

	//���Ȼ�ȡ���ͺ͸�ʴ������Ҫ�� �Զ����element
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//ͨ��ͨ��getStructuringElement(�ں���״��MORPH_RECT�����Σ�MORPH_CROSS�������ͣ�MORPH_ELLIPSE����Բ�Σ�;�ߴ�;ê��λ��Ĭ��Point(-1,-1)��ʾλ������)
	
	//��˹ģ������ʴ������
	GaussianBlur(foreground_threshold, foreground_threshold, Size(3, 3), 0, 0);
	erode(foreground_threshold, foreground_x, element);
	//dilate(foreground_x, foreground_x, element);
	//dilate(foreground_x, foreground_x, element);
	//erode(foreground_threshold, foreground_x, element);

	//Ϊ��ʾЧ��������������
	pyrUp(foreground_x, foreground_x); 
	pyrUp(foreground_x, foreground_x);
	pyrUp(foreground_x, foreground_x);
	pyrUp(video_t, video_t);
	pyrUp(video_t, video_t);
	pyrUp(video_t, video_t);
	
	//׼�����λ��ƣ����ȶ�����ɫ����color
	Scalar color(0, 0, 255);

	Mat contours = Mat::zeros(foreground_x.rows, foreground_x.cols, CV_8UC3); //����ͼ�������Ϊ����
	
	findContours(foreground_x, contours_v, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//contours_v�д洢��ձ�Ե

	drawContours(contours, contours_v, -1, color);//���Ʊ�Ե

	//��ȡcontours_v�е�ÿ����Ե����������Ӿ���r�����r������������ʾ�ڽ��������
	for (int i = 0; i <= contours_v.size()-1;i++)
	{
		Rect r = boundingRect(Mat(contours_v[i]));
		if (r.width > 150 && r.height > 120)
		{
			if (r.y > 500)
			{
				rect = r;
				rect.width = 450;
				rect.height = 200;
				rect.x += 50;
				circle(video_t, Point(rect.x+150, rect.y+100), 20, Scalar(0, 0, 255),-1);
			}
			else if (r.y <= 200)
			{
				rect = r;
				//rect.x = (r.x+r.width)*0.2;
				//if (rect.x < 0) rect.x = 0;
				if (r.height > 400)
					rect.y = r.y+r.height - 500;
				//if (rect.y < 0) rect.y = 0;
				rect.height = 450;
				rect.width = 200;
				circle(video_t, Point(rect.x + 80, rect.y + rect.height*0.1), 20, Scalar(0, 0, 255), -1);

			}
			else
			{
				rect = r;
				rect.x += 50;
				rect.width -= 50;
				circle(video_t, Point(rect.x + 100, rect.y + rect.height*0.2), 20, Scalar(0, 0, 255), -1);
			}
			rectangle(video_t, rect, Scalar(0, 255, 0), 2);
			cout << r.y << endl;
		}	
	}
	
	return video_t;
}

Mat jiaozheng(Mat image)
{
	Size image_size = image.size();
	//����У������3*3 [fx,0,cx;0,fy,cy;0,0,1]
	float intrinsic[3][3] = { 878.66998, 0, 640.45135, 0, 881.11673, 382.92259, 0, 0, 1 };
	float distortion[1][5] = { -0.36996, 0.10612, -0.00733, 0.01104, 0 };
	Mat intrinsic_matrix = Mat(3, 3, CV_32FC1, intrinsic);
	Mat distortion_coeffs = Mat(1, 5, CV_32FC1, distortion);
	Mat R = Mat::eye(3, 3, CV_32F);
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);
	initUndistortRectifyMap(intrinsic_matrix, distortion_coeffs, R, intrinsic_matrix, image_size, CV_32FC1, mapx, mapy);
	Mat t = image.clone();
	cv::remap(image, t, mapx, mapy, INTER_LINEAR);
	return t;
}


Mat PerspectiveTransformation_1(Mat not_a_rect_photo)
{

	vector<Point> not_a_rect_shape;
	not_a_rect_shape.push_back(Point(556 / 4, 37 / 4));
	not_a_rect_shape.push_back(Point(860 / 4, 35 / 4));
	not_a_rect_shape.push_back(Point(249 / 4, 589 / 4));
	not_a_rect_shape.push_back(Point(1232 / 4, 626 / 4));

	cv::Point2f src_vertices[4];
	src_vertices[0] = not_a_rect_shape[0];
	src_vertices[1] = not_a_rect_shape[1];
	src_vertices[2] = not_a_rect_shape[2];
	src_vertices[3] = not_a_rect_shape[3];

	Point2f dst_vertices[4];
	dst_vertices[0] = Point(100, 0);
	dst_vertices[1] = Point(100, 40);
	dst_vertices[2] = Point(0, 0);
	dst_vertices[3] = Point(0, 40);
	Mat warpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	cv::Mat rotated;
	warpPerspective(not_a_rect_photo, rotated, warpMatrix, rotated.size(), INTER_LINEAR, BORDER_CONSTANT);
	Mat roi_rotated = rotated(Range(0, 40), Range(0, 100));
	return roi_rotated;
}

Mat PerspectiveTransformation_2(Mat not_a_rect_photo)
{

	vector<Point> not_a_rect_shape;
	not_a_rect_shape.push_back(Point(499 / 4, 101 / 4));
	not_a_rect_shape.push_back(Point(734 / 4, 107 / 4));
	not_a_rect_shape.push_back(Point(384 / 4, 384 / 4));
	not_a_rect_shape.push_back(Point(849 / 4, 381 / 4));

	cv::Point2f src_vertices[4];
	src_vertices[0] = not_a_rect_shape[0];
	src_vertices[1] = not_a_rect_shape[1];
	src_vertices[2] = not_a_rect_shape[2];
	src_vertices[3] = not_a_rect_shape[3];

	Point2f dst_vertices[4];
	dst_vertices[0] = Point(0, 0);
	dst_vertices[1] = Point(30, 0);
	dst_vertices[2] = Point(0, 60);
	dst_vertices[3] = Point(30, 60);
	Mat warpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	cv::Mat rotated;
	warpPerspective(not_a_rect_photo, rotated, warpMatrix, rotated.size(), INTER_LINEAR, BORDER_CONSTANT);
	Mat roi_rotated = rotated(Range(0, 60), Range(0, 30));
	return roi_rotated;
}

Mat PerspectiveTransformation_3(Mat not_a_rect_photo)
{

	vector<Point> not_a_rect_shape;
	not_a_rect_shape.push_back(Point(516 / 4, 73 / 4));
	not_a_rect_shape.push_back(Point(802 / 4, 77 / 4));
	not_a_rect_shape.push_back(Point(347 / 4, 388 / 4));
	not_a_rect_shape.push_back(Point(1000 / 4, 388 / 4));

	cv::Point2f src_vertices[4];
	src_vertices[0] = not_a_rect_shape[0];
	src_vertices[1] = not_a_rect_shape[1];
	src_vertices[2] = not_a_rect_shape[2];
	src_vertices[3] = not_a_rect_shape[3];

	Point2f dst_vertices[4];
	dst_vertices[0] = Point(50, 0);
	dst_vertices[1] = Point(50, 40);
	dst_vertices[2] = Point(0, 0);
	dst_vertices[3] = Point(0, 40);
	Mat warpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	cv::Mat rotated;
	warpPerspective(not_a_rect_photo, rotated, warpMatrix, rotated.size(), INTER_LINEAR, BORDER_CONSTANT);
	Mat roi_rotated = rotated(Range(0, 40), Range(0, 50));
	return roi_rotated;
}


int main()
{
	Mat background_global = imread("1.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

	VideoCapture capture_1("out_1.avi");
	VideoCapture capture_2("out_2.avi");
	VideoCapture capture_3("out_4.avi");

	Mat frame_1;
	Mat frame_2;
	Mat frame_3;

	Mat frame_target(100, 150, CV_8UC3,Scalar(255,255,255));
	Mat frame_detection;

	Mat RoI_1(frame_target, Rect(50, 60, 100, 40));
	Mat RoI_2(frame_target, Rect(0, 0, 30, 60));
	Mat RoI_3(frame_target, Rect(0, 60, 50, 40));

	while (1)
	{
		capture_1 >> frame_1;
		capture_2 >> frame_2;
		capture_3 >> frame_3;

		if (frame_1.empty() || frame_2.empty() || frame_3.empty())
		{
			break;
		}

		frame_1 = jiaozheng(frame_1);
		pyrDown(frame_1, frame_1);
		pyrDown(frame_1, frame_1);
		frame_1 = PerspectiveTransformation_1(frame_1);

		frame_2 = jiaozheng(frame_2);
		pyrDown(frame_2, frame_2);
		pyrDown(frame_2, frame_2);
		frame_2 = PerspectiveTransformation_2(frame_2);

		frame_3 = jiaozheng(frame_3);
		pyrDown(frame_3, frame_3);
		pyrDown(frame_3, frame_3);
		frame_3 = PerspectiveTransformation_3(frame_3);

		frame_1.copyTo(RoI_1);
		frame_2.copyTo(RoI_2);
		frame_3.copyTo(RoI_3);

		imshow("���ͼ1", frame_1);
		imshow("���ͼ2", frame_2);
		imshow("���ͼ3", frame_3);
		imshow("���ͼ", frame_target);

		frame_detection=Diff_and_Detect(frame_target, background_global);
		
		imshow("�����", frame_detection);

		waitKey(50);
	}
	return 0;
}

