    // 함수를 선언합니다.
    function callTenTimes(callback) {
        // 10회 반복합니다.
        for (var i = 0; i < 10; i++) {
            // 매개 변수로 전달된 함수를 호출합니다.
            console.log(`i = ${i}`)
            callback();
        }
    }

    // 변수를 선언합니다.
    var abc = function () {
        console.log('함수 호출');
    };
    abc();
    // 함수를 호출합니다.
    callTenTimes(abc);
