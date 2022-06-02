    // Date 객체를 선언합니다: 현재 시간 측정
    var date = new Date();

    // 요소를 추출합니다.
    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    var day = date.getDay();
    var hours = date.getHours();
    var minutes = date.getMinutes();
    var seconds = date.getSeconds();

    console.log("year = %d", year);
    console.log("month = %d", month);
    console.log("day = %d", day);
    console.log("hours = %d", hours);
    console.log("minutes = %d", minutes);
    console.log("seconds = %d", seconds);