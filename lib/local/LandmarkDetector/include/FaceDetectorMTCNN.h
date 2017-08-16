///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016, Carnegie Mellon University and University of Cambridge,
// all rights reserved.
//
// THIS SOFTWARE IS PROVIDED �AS IS� FOR ACADEMIC USE ONLY AND ANY EXPRESS
// OR IMPLIED WARRANTIES WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY.
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Notwithstanding the license granted herein, Licensee acknowledges that certain components
// of the Software may be covered by so-called �open source� software licenses (�Open Source
// Components�), which means any software licenses approved as open source licenses by the
// Open Source Initiative or any substantially similar licenses, including without limitation any
// license that, as a condition of distribution of the software licensed under such license,
// requires that the distributor make the software available in source code format. Licensor shall
// provide a list of Open Source Components for a particular version of the Software upon
// Licensee�s request. Licensee will comply with the applicable terms of such licenses and to
// the extent required by the licenses covering Open Source Components, the terms of such
// licenses will apply in lieu of the terms of this Agreement. To the extent the terms of the
// licenses applicable to Open Source Components prohibit any of the restrictions in this
// License Agreement with respect to such Open Source Component, such restrictions will not
// apply to such Open Source Component. To the extent the terms of the licenses applicable to
// Open Source Components require Licensor to make an offer to provide source code or
// related information in connection with the Software, such offer is hereby made. Any request
// for source code or related information should be directed to cl-face-tracker-distribution@lists.cam.ac.uk
// Licensee acknowledges receipt of notices for the Open Source Components for the initial
// delivery of the Software.

//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite at least one of the following works:
//
//       OpenFace: an open source facial behavior analysis toolkit
//       Tadas Baltru�aitis, Peter Robinson, and Louis-Philippe Morency
//       in IEEE Winter Conference on Applications of Computer Vision, 2016  
//
//       Rendering of Eyes for Eye-Shape Registration and Gaze Estimation
//       Erroll Wood, Tadas Baltru�aitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, and Andreas Bulling 
//       in IEEE International. Conference on Computer Vision (ICCV),  2015 
//
//       Cross-dataset learning and person-speci?c normalisation for automatic Action Unit detection
//       Tadas Baltru�aitis, Marwa Mahmoud, and Peter Robinson 
//       in Facial Expression Recognition and Analysis Challenge, 
//       IEEE International Conference on Automatic Face and Gesture Recognition, 2015 
//
//       Constrained Local Neural Fields for robust facial landmark detection in the wild.
//       Tadas Baltru�aitis, Peter Robinson, and Louis-Philippe Morency. 
//       in IEEE Int. Conference on Computer Vision Workshops, 300 Faces in-the-Wild Challenge, 2013.    
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __FACE_DETECTOR_MTCNN_h_
#define __FACE_DETECTOR_MTCNN_h_

// OpenCV includes
#include <opencv2/core/core.hpp>

// System includes
#include <vector>

using namespace std;

namespace LandmarkDetector
{
	class CNN
	{
	public:

		//==========================================

		// Default constructor
		CNN() { ; }

		// Copy constructor
		CNN(const CNN& other);

		// Given an image, orientation and detected landmarks output the result of the appropriate regressor
		std::vector<cv::Mat_<float> > Inference(const cv::Mat& input_img);

		// Reading in the model
		void Read(string location);

		// Precomputations for faster convolution
		vector<vector<vector<pair<int, cv::Mat_<double> > > > > cnn_convolutional_layers_dft;
		vector<vector<map<int, vector<cv::Mat_<double> > > > > cnn_convolutional_layers_dft2;

	private:
		//==========================================
		// Convolutional Neural Network

		// CNN layers
		// layer -> input maps -> kernels
		// Bit ugly with so much nesting, but oh well
		vector<vector<vector<cv::Mat_<float> > > > cnn_convolutional_layers;
		vector<vector<vector<cv::Mat_<float> > > > cnn_convolutional_layers_rearr;
		vector<vector<float > > cnn_convolutional_layers_bias;
		vector<cv::Mat_<float> >  cnn_fully_connected_layers_weights;
		vector<cv::Mat_<float> > cnn_fully_connected_layers_biases;
		vector<cv::Mat_<float> >  cnn_prelu_layer_weights;
		vector<std::tuple<int, int, int, int> > cnn_max_pooling_layers;

		// CNN: 0 - convolutional, 1 - max pooling, 2 - fully connected, 3 - prelu, 4 - sigmoid
		vector<int > cnn_layer_types;
	};
	//===========================================================================
	//
	// Checking if landmark detection was successful using an SVR regressor
	// Using multiple validators trained add different views
	// The regressor outputs -1 for ideal alignment and 1 for worst alignment
	//===========================================================================
	class FaceDetectorMTCNN
	{

	public:

		// Default constructor
		FaceDetectorMTCNN() { ; }

		// Copy constructor
		FaceDetectorMTCNN(const FaceDetectorMTCNN& other);

		// Given an image, orientation and detected landmarks output the result of the appropriate regressor
		bool DetectFaces(vector<cv::Rect_<double> >& o_regions, const cv::Mat& input_img, std::vector<double>& o_confidences, int min_face = 30, double t1 = 0.6, double t2 = 0.7, double t3 = 0.7);

		// Reading in the model
		void Read(string location);

	private:
		//==========================================
		// Components of the model

		CNN PNet;
		CNN RNet;
		CNN ONet;
		
	};

}
#endif
