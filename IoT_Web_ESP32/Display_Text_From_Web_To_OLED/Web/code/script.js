// Link DB: https://console.firebase.google.com/project/dispaly-text-on-oled/database/dispaly-text-on-oled-default-rtdb/data/~2F?fb_gclid=CjwKCAjwobnGBhBNEiwAu2mpFG_DHxc8tUyLRM-cPEq3KFkLoCiq6UrC-eQ3ob5o_tya5bsPIIo-5xoCkfMQAvD_BwE

// script.js (non-module, dùng global `firebase`)

// ====== Cấu hình Firebase - thay bằng config của bạn nếu khác ======
const firebaseConfig = {
  apiKey: "AIzaSyAlLE9pJW6xdLziBAa2KJxbTTWnI9TbI7w",
  authDomain: "dispaly-text-on-oled.firebaseapp.com",
  projectId: "dispaly-text-on-oled",
  storageBucket: "dispaly-text-on-oled.appspot.com",
  messagingSenderId: "51600372864",
  appId: "1:51600372864:web:3cf88e9114ab1a522d6265",
};

// Khởi tạo Firebase
firebase.initializeApp(firebaseConfig);

// Tham chiếu đến DB
const database = firebase.database();

// ====== DOM elements ======
const toggleSwitch = document.getElementById("toggle-switch");
const textInput = document.getElementById("text-input");
const clearBtn = document.getElementById("clear-btn");
const okBtn = document.getElementById("ok-btn");


// Các hàm xử lý sự kiện

// Biến cờ để theo dõi trạng thái sau khi BTN_DELETE được set
let isTextClearedByBtnDelete = false;

// Hàm cập nhật trạng thái disabled
function updateControlsState(isEnabled) {
  textInput.disabled = !isEnabled; // Vô hiệu hóa ô nhập liệu khi isEnabled là false
  clearBtn.disabled = !isEnabled; // Vô hiệu hóa button "Xóa"
  okBtn.disabled = !isEnabled; // Vô hiệu hóa button "OK"
}

// Khởi tạo trạng thái ban đầu (tùy thuộc vào trạng thái switch khi tải trang)
updateControlsState(toggleSwitch.checked); // Gọi lần đầu để đặt trạng thái ban đầu

// Ghi dữ liệu từ web lên firebase
// Xử lý switch
toggleSwitch.onchange = function() {
  const switchValue = toggleSwitch.checked ? 1 : 0;
  database.ref("/DISPLAY_TEXT").update({
    "SWITCH": switchValue
  });
  updateControlsState(switchValue === 1); // Cập nhật trạng thái dựa trên switchValue
};

// Xử lý button "Xóa"
clearBtn.onclick = function() {
  // Xóa chuỗi text trong ô nhập liệu
  textInput.value = "";
  // Gửi tín hiệu BTN_DELETE lên Firebase
  database.ref("/DISPLAY_TEXT").update({
    "BTN_DELETE": 1
  });
  setTimeout(() => {
    database.ref("/DISPLAY_TEXT").update({
      "BTN_DELETE": 0
    });
  }, 500); // 500ms = 0.5s
};

// Xử lý button "OK" để gửi text lên Firebase
okBtn.onclick = function() {
  const textValue = textInput.value.trim(); // Lấy giá trị từ ô nhập liệu và loại bỏ khoảng trắng thừa
  database.ref("/DISPLAY_TEXT").update({
    "BTN_OK": 1,
    "TEXT": textValue // Cập nhật chuỗi text vào trường "TEXT"
  });
  setTimeout(() => {
    database.ref("/DISPLAY_TEXT").update({
      "BTN_OK": 0
    });
  }, 500); // 500ms = 0.5s
};

// Đồng bộ dữ liệu từ DB về web
database.ref("/DISPLAY_TEXT").on("value", function (snapshot) {
  const data = snapshot.val() || {}; // Lấy dữ liệu, mặc định là object rỗng nếu null
  const switchValue = data["SWITCH"] || 0; // Lấy giá trị SWITCH
  const btnDeleteValue = data["BTN_DELETE"] || 0; // Lấy giá trị BTN_DELETE
  const textValue = data["TEXT"] || ""; // Lấy giá trị TEXT, mặc định rỗng nếu không tồn tại

  // Cập nhật trạng thái switch và controls dựa trên DB
  toggleSwitch.checked = switchValue === 1; // Đồng bộ trạng thái switch
  if (switchValue === 1) {
    updateControlsState(true); // Kích hoạt các phần tử
  } else {
    updateControlsState(false); // Vô hiệu hóa các phần tử
    textInput.value = ""; // Xóa nội dung ô nhập liệu khi switch tắt
  }

  // Hiển thị text từ DB lên ô nhập liệu, trừ khi switch tắt hoặc đã bị xóa bởi BTN_DELETE
  if (switchValue === 1 && !isTextClearedByBtnDelete) {
    textInput.value = textValue; // Cập nhật giá trị ô nhập liệu với TEXT từ DB
  }

  // Xử lý xóa text trên web khi BTN_DELETE là 1
  if (btnDeleteValue === 1) {
    textInput.value = ""; // Xóa nội dung ô nhập liệu
    isTextClearedByBtnDelete = true; // Đánh dấu rằng text đã bị xóa bởi BTN_DELETE
    setTimeout(() => {
      database.ref("/DISPLAY_TEXT").update({
        BTN_DELETE: 0,
      });
      isTextClearedByBtnDelete = false; // Reset cờ sau khi BTN_DELETE về 0
    }, 500); // 500ms = 0.5s
  }
});
