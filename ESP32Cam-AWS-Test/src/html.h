#ifndef MY_HTML_H
#define MY_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/cropperjs/1.6.0/cropper.css" integrity="sha512-hBT8ir1rdidrjzYcDj50eQ5gglkAc1c7tmzpdkiGydrn3Tt3+cGJ+GdsXLYmwETdStY0JZXbv6p4jgGDQU0fMw==" crossorigin="anonymous" referrerpolicy="no-referrer" />
        <style>
            .header {
                text-align:center; 
            }
            main{
                max-width: 1280px;
                margin: 20px auto;
            }
            .button-container{
                text-align: center;
            }

            .crop-container{
                max-width: 1280px;
                margin: 20px auto;
            }
            img {
                width: 100%;
            }
        </style>
    </head>
    <body>
        <header class="header">
            <h2>ESP32-CAM</h2>
            <p>Press take photo to get image from ESP32-Cam, then press check photo to display image.</p>
            <p>
                <button onclick="capturePhoto()">Take Photo</button>
                <button onclick="location.reload();">Check Photo</button>
            </p>
        </header>
        <main>
            <div class="crop-container">
                <img src="saved-photo" id="photo">
            </div>
            <div class="button-container">
                <p>Press take photo to get image from ESP32-Cam, then press check photo to display image.</p>
                <p>
                    <button type="button" id="crop-button">Preview Crop Photo</button>
                    <button type="button" id="send-crop-data">Save Crop Dimensions</button>
                    <button type="button" id="send-aws">Send AWS</button>
                </p>
            </div>
            <div class="crop-data">
                <h3>Data:</h3>
                <p><span id="cropper-data"></span></p>
                <p><span id="esp32-crop"></span></p>
            </div>
            <h3>Result</h3>
            <div id="crop-result"></div>
        </main>
        <!-- TesseractJS v4 -->
        <script src='https://cdn.jsdelivr.net/npm/tesseract.js@4/dist/tesseract.min.js'></script>
        <script src="https://code.jquery.com/jquery-3.7.0.min.js" integrity="sha256-2Pmvv0kuTBOenSvLm6bvfBSSHrUJ+3A7x6P5Ebd07/g=" crossorigin="anonymous"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/cropperjs/1.6.0/cropper.js" integrity="sha512-q9N9RDpXWA3HKu2spOTpZ0j64hL0kfwHQiCgGq9FUuVtTOn7K894cu3YQBtA6rz8T7gbAyuBo+sooxR+/sO4Lg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    </body>
    <script>
        window.addEventListener('DOMContentLoaded', function () {
            let image = document.querySelector('#photo');
            let cropPreview = document.getElementById('crop-button');
            let previewResult = document.getElementById('crop-result');
            let sendCropData = document.getElementById('send-crop-data');
            let sendAWSButton = document.getElementById('send-aws')
            let cropper = new Cropper(image, { 
                ready: function (event) {
                // Zoom the image to its natural size
                cropper.zoomTo(1);
                },
                

                crop: function (event) {
                    let cropData = cropper.getData();
                    // document.querySelector('#cropper-data').textContent = JSON.stringify(cropper.getData());
                    document.querySelector('#esp32-crop').textContent = `Crop-Left: ${Math.round(cropData.x)} 
                                                                        Crop-Top: ${Math.round(cropData.y)} 
                                                                        Crop-Height: ${Math.round(cropData.height)} 
                                                                        Crop-Width: ${Math.round(cropData.width)}`;
                },

                zoom: function (event) {
                    // Keep the image in its natural size
                    if (event.detail.oldRatio === 1) {
                        event.preventDefault();
                    }
                },
            });

            cropPreview.onclick = () => {
                previewResult.innerHTML = '';
                previewResult.appendChild(cropper.getCroppedCanvas());
            };

            sendCropData.onclick = () => {
                // convert crop data and store in json
                let cropData = cropper.getData();
                let cropLeft = Math.round(cropData.x);
                let cropTop = Math.round(cropData.y);
                let cropBottom = Math.round(image.naturalHeight - (cropData.y+cropData.height));
                let cropRight = Math.round(image.naturalWidth - (cropData.x+cropData.width));
                let cropHeight = cropData.height;
                let cropWidth = cropData.width;
                let xhr = new XMLHttpRequest();
                xhr.open('GET', `/cropData?cropLeft=${cropLeft}&cropTop=${cropTop}&cropWidth=${cropWidth}&cropHeight=${cropHeight}`, true);
                xhr.send();
                
            }
            sendAWSButton.onclick = () => {
                console.log("Tell ESP32 to send to AWS");
                let xhr = new XMLHttpRequest();
                xhr.open('GET', "/send-aws", true);
                xhr.send();
            }
        });
        let capturePhoto = () => {
            let xhr = new XMLHttpRequest();
            xhr.open('GET', "/capture", true);
            xhr.send();
        }
    </script>
</html>
)rawliteral";

#endif