const arr1 = [1,2,3];

let arr = [1, ...arr1, 2];
for( i=0;i<arr.length;i++){
    // console.log(arr[i]);
    console.log(`arr[${i}] : ${arr[i]}`);
}