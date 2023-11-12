<script setup>

import CellularIcon from "./CellularIcon.vue";
import WifiIcon from "./WifiIcon.vue";
</script>

<template>
  <ul class="nav col-12 col-md-auto mb-2 justify-content-center mb-md-0">
    <WifiIcon :connected="true" :rssi="-50"/>
    <CellularIcon :connected="true"/>
    <Icon name="gi-sattelite" scale="1.5"/>
    <h5 class="mb-0">4</h5>
  </ul>
</template>

<style scoped>

</style>
