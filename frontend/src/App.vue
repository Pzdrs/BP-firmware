<script setup>

import axios from "axios";

function doShit() {
  axios.get("https://jsonplaceholder.typicode.com/todos/1")
    .then(response => {
      console.log(response.data);
    })
    .catch(error => {
      console.log(error);
    });
}
</script>

<template>
  <button @click="doShit" class="btn btn-outline-danger">asdfasdfasdfasd</button>
</template>

<style scoped>

</style>
