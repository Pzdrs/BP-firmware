<script setup>
import {ref} from "vue";

const positionFixed = ref(true);

const socket = new WebSocket(`ws://10.30.0.110/gnss`);
socket.onopen = () => {
  console.log('connected');
};
socket.onmessage = (event) => {
  console.log(event.data);
};

</script>

<template>
  <section class="text-center">
    <h2 v-if="positionFixed" class="text-success">POSITION FIXED</h2>
    <h2 v-else class="text-danger">NO FIX</h2>
  </section>
  <hr>
  <section class="d-flex justify-content-between">
    <section>
      <b>Longitude:</b><br>
      <b>Latitude:</b><br>
      <b>Altitude:</b><br>
    </section>
  </section>

</template>

<style scoped>

</style>
