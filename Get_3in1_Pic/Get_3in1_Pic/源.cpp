#include <opencv2\opencv.hpp>
#include <imgproc\imgproc.hpp>
using namespace cv;

Mat frame_traget(100, 150, CV_8UC3,Scalar(255,255,255));

void on_MouseHandle(int event, int x, int y, int flags, void* param)
{
	static int n = 0;
	//Mat& image = *(cv::Mat*) param;
	switch (event)
	{

		//×ó¼ü°´ÏÂÏûÏ¢
	case EVENT_LBUTTONDOWN:
	{
							  n++;
							  Mat mtx(frame_traget);
							  switch (n)
							  {
							  case 1:
							  {
										imwrite("1.jpg", mtx);
							  }
								  break;
							  case 2:
							  {
										imwrite("2.jpg", mtx);
							  }
								  break;
							  case 3:
							  {
										imwrite("3.jpg", mtx);
							  }
								  break;
							  case 4:
							  {
										imwrite("4.jpg", mtx);
							  }
								  break;
							  case 5:
							  {
										imwrite("5.jpg", mtx);
							  }
								  break;
							  case 6:
							  {
										imwrite("6.jpg", mtx);
							  }
								  break;
							  case 7:
							  {
										imwrite("7.jpg", mtx);
							  }
								  break;
							  case 8:
							  {
										imwrite("8.jpg", mtx);
							  }
								  break;
							  case 9:
							  {
										imwrite("9.jpg", mtx);
							  }
								  break;
							  case 10:
							  {
										 imwrite("10.jpg", mtx);
							  }
								  break;
							  case 11:
							  {
										 imwrite("11.jpg", mtx);
							  }
								  break;
							  case 12:
							  {
										 imwrite("12.jpg", mtx);
							  }
								  break;
							  case 13:
							  {
										 imwrite("13.jpg", mtx);
							  }
								  break;
							  case 14:
							  {
										 imwrite("14.jpg", mtx);
							  }
								  break;
							  case 15:
							  {
										 imwrite("15.jpg", mtx);
							  }
								  break;
							  }

	}
		break;


	}
}

Mat jiaozheng(Mat image)
{
	Size image_size = image.size();
	//»û±äÐ£Õý¾ØÕó3*3 [fx,0,cx;0,fy,cy;0,0,1]
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

	VideoCapture capture_1("out_1.avi");
	VideoCapture capture_2("out_2.avi");
	VideoCapture capture_3("out_4.avi");

	Mat frame_1;
	Mat frame_2;
	Mat frame_3;

	Mat RoI_1(frame_traget, Rect(50, 60, 100, 40));
	Mat RoI_2(frame_traget, Rect(0, 0, 30, 60));
	Mat RoI_3(frame_traget, Rect(0, 60, 50, 40));

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

		frame_traget.at<Vec3b>(0, 149) = (0,0,0);
		//pyrUp(end,end);
		//pyrUp(end, end);
		//imshow("Äñî«Í¼1", frame_1);
		//imshow("Äñî«Í¼2", frame_2);
		//imshow("Äñî«Í¼3", frame_3);
		imshow("Äñî«Í¼", frame_traget);

		setMouseCallback("Äñî«Í¼", on_MouseHandle);

		waitKey(50);
		/**
		if (frame_1.empty()||frame_2.empty())
		{
		break;
		}
		**/
	}

	return 0;
}

