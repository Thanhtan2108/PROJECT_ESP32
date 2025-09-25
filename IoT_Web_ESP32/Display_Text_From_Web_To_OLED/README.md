# Tạo WEB

## Tạo nhanh cấu trúc 1 trang web

1. Cách bật Emmet trong VS Code

Mở Settings (Ctrl + ,).

Tìm: Emmet: Trigger Expansion On Tab.

Bật lên (tick chọn).

Đảm bảo file bạn đang gõ có đuôi .html hoặc .htm (hoặc trong VS Code status bar phải nhận diện là HTML). Nếu đang ở file .txt thì Emmet sẽ không chạy.

2. Tạo nhanh cấu trúc code của HTML

Chỉ cần gõ trong file .html:

```html
html:5  <!-- hoặc ! -->
```

rồi nhấn Tab → nó sẽ tự sinh ra bộ khung HTML5 chuẩn:

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Document</title>
</head>
<body>
  
</body>
</html>
```

3. Để tạo cấu trúc web

gõ nhanh Emmet như sau trong phần `<body>`:

```html
header+nav+main>section.content+aside.sidebar^footer
```

rồi nhấn Tab, nó sẽ sinh ra:

```html
<header></header>
<nav></nav>
<main>
  <section class="content"></section>
  <aside class="sidebar"></aside>
</main>
<footer></footer>
```

## Tạo nhanh các liên kết, các khối, các component trong web

1. Các cú pháp Emmet:

  + → tạo anh em cùng cấp

  > → tạo con bên trong

  ^ → nhảy lên cấp cha

  .class → thêm class

  #id → thêm id

2. Tạo nhanh liên kết file css

Trong phần `<head>...</head>`, bạn chỉ cần gõ:

```html
link:css
```

rồi nhấn Tab → nó sẽ tự sinh ra:

```html
<link rel="stylesheet" href="style.css">
```

3. Để dùng các icon Font Awesome

Trong phần `<head>...</head>`, chèn link CDN vào:

```html
    <link
    rel="stylesheet"
    href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/[version]/css/all.min.css">
```

Di chuyển đến trang icon Font Awesome

[Trang chọn icon miễn phí](https://fontawesome.com/search?new=yes&o=r)

Chọn version tương ứng với version đã dùng ở phần header

Tìm Icon muốn dùng, có thể dùng ô tìm kiếm

Chọn loại HTML và copy code trong thẻ <i ...></i> được cung cấp sẵn

Paste và code trong html để sử dụng

Trong phần cuối `<body>...</body>`, chèn code dẫn đến file js:

```html
<script src="script.js"></script>
```

## Viết code js xử lý

Bên HTML tạo id cho các đối tượng/sự kiện cần được tác động

Khởi tạo các phần tử DOM bằng cách tạo các biến liên kết với các đối tường cần tác động/xử lý sự kiện bên HTML

Thông thường tạo biến và id cùng tên

```js
const variable = document.getElementById("id của đối tượng/sự kiện đó");
```

Viết các hàm tiện ích/xử lý sự kiện/dữ liệu bằng DOM Events

Xử lý sự kiện khi nhấn vào button

```js
variable.onclick = function(){
  // giải thuật xử lý 
}
```

Xử lý sự kiện khi nhấn vào switch

```js
variable.onchange = function() {
  const variableValue = variable.checked ? 1 : 0;
  database.ref("/đường dẫn đến thư mục quản lý trên DB").update({
    "SWITCH": switchValue
  });
};
```

Cài đặt thời gian TimeOut cho 1 sự kiện

```js
setTimeout(() => {
  // giải thuật nếu có
  database.ref("/thư mục quản lý trên DB").update({
    "biến trên DB": value
  });
}, Timeout_ms);
```

Load hình vào khi có sự kiện tác động

```js
variable.src = "đường dẫn đến ảnh";
```

Lấy giá trị/nội dung của 1 element và hiển thị lên web bằng

```js
variable.innerHTML
```

Xuất ra console để debug nếu cần

```js
console.log("variable");
```

Giá trị hiện tại từ ô nhập liệu `<input type="text" name="text">`

```js
const textValue = textInput.value.trim();
```

  Với:

  textInput.value: giá trị hiện tại từ ô nhập liệu.
  
  .trim(): Loại bỏ khoảng trắng thừa ở đầu và cuối chuỗi để tránh lưu dữ liệu rỗng không mong muốn.

Vô hiệu hóa 1 element để không thể tác động đến nó

```js
variable.disabled = false;
```

sẽ `vô hiệu hóa phần tử` khi được đặt là `true`, và `kích hoạt` khi được đặt là `false`

## Liên kết với firebase database, Realtime database theo kiểu không có dùng module

### Có thể tham khảo tìm hiểu cách dùng module có inport thư viện bằng chat GPT

Truy cập vào trang chủ firebase

[Trang chủ firebase](https://console.firebase.google.com)

Và đăng nhập bằng tài khoản google

Chọn `Create a project`

Nhập tên cho project cần tạo, chú ý `chỉ được dùng các chữ cái, dấu ! và -` thôi

Chọn `Continue`

`Bỏ chọn Enable Google Analytics for this project`

Chọn `Create project`

Chờ tạo xong database và chọn `Continue`

Sau đó liên kết database với web bằng cách bấm `Add App` và chọn `Web`

Đặt tên cho web và bỏ chọn `Also set up Firebase Hosting for this app`

Chọn `Register`

Sau đó copy đoạn code

```js
const firebaseConfig ={...};
```

Có thể xem lại code config này tại `setting project`

Sau đó paste ở đầu của `file.js` để liên kết

Bấm `Continue to console`

(Có thể Add thêm cho điện thoại)

Tiếp theo liên kết với Realtime database bằng cách chọn `Build` tìm chọn `Realtime Database`

Chọn `Create Database`

Chọn `location` là ở `US` và nhấn `Next`

Chọn `Enable` ở mục cấp quyền

Sau khi tạo xong Realtime Database, chọn tab `Rule` và sửa các giá trị từ `false` thành `true`
để cho phép được đọc ghi

Sau đó lưu lại bằng chọn `Public` rồi có thể chọn `Dismiss`

Quay lại tab `Data` nơi chứa data liên kết từ web

Tạo thư mục quản lý và biến trên database để chứa dữ liệu bằng dấu `+`

Sau đó nhấn `Add` để chấp nhận lưu thư mục và biến

Bắt đầu khởi động để web giao tiếp với database

Khởi tạo Firebase

```js
// Khởi tạo Firebase (lần đầu)
if (!firebase.apps || !firebase.apps.length) {
  firebase.initializeApp(firebaseConfig);
} else {
  // nếu đã khởi tạo (hiếm khi xảy ra)
  firebase.app();
}
```

Khởi tạo biến để liên kết với Realtime Database và tham chiếu dữ liệu từ service

```js
const database = firebase.database();
```

Đặt vào bên dưới code liên kết web với firebase

Chỉ định đường dẫn thư mục quản lý dữ liệu trên Database

```js
// Đường dẫn root muốn dùng được tạo trên Realtime Database
const rootPath = "DISPLAY_TEXT";
const rootRef = database.ref(rootPath);
```

Quay lại `file.html` thêm thư viện để sử dụng firebase được và phải để trước source code script trong `<body></body>`

```html
  <!-- Firebase SDK -->
  <script src="https://www.gstatic.com/firebasejs/10.12.5/firebase-app.js"></script>
  <script src="https://www.gstatic.com/firebasejs/10.12.5/firebase-database.js"></script>
```

(Có thể chọn version khác cho thư viện)

## Đọc/ghi data từ web lên database

1. Ghi data từ web lên firebase

Cần cập nhật giá trị của 1 biến trên database khi có tác động từ web

Trong hàm xử lý sự kiện

```js
database.ref("/đường dẫn tương ứng với biến trên Realtime Database").update({
  "Biến trên Realtime Database" : "value"
})
```

2. Đọc dữ liệu liên tục từ firebase về web

Đọc data liên tục từ firebase về web có 2 loại:

Đọc trực tiếp từ biến trong 1 thư mục quản lý trên Realtime Database

```js
database.ref("/đường dẫn đến biến trên Realtime Database").on("value", funtion(snapshot){
  //đọc dữ liệu về từ firebase đã được lưu trong biến snapshot vầ lưu lại 1 biến tạm i trên web
  let i = snapshot.val();
  // hoặc xuất ra HTML
  variable.innerHTML = i;
  // hoặc xử lý sự kiện từ web bằng dữ liệu đã được lưu trong biến tạm i bằng các lấy giá trị trực tiếp của biến i để xử lý
  if(t)
})
```

Đọc gián tiếp từ thư mục quản lý trên Realtime Database

```js
database.ref("/thư mục quản lý trên DB").on("value", function(snapshot){
  //đọc dữ liệu về từ firebase đã được lưu trong biến snapshot vầ lưu lại 1 biến tạm i trên web dưới dạng Array {key:value, key:value, ...}
  let i = snapshot.val();
  // hoặc xuất ra HTML
  variable.innerHTML = i;
  // hoặc xử lý sự kiện từ web bằng dữ liệu đã được lưu trong biến tạm i bằng cách truy cập đến key được lưu trong mảng i và xử lý
  if(i[key])
})
```
