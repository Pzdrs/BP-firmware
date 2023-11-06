<script setup>

import WifiIcon from "./components/WifiIcon.vue";
import CellularIcon from "./components/CellularIcon.vue";

</script>

<template>
  <div class="container-fluid">
    <header
        class="d-flex flex-wrap align-items-center justify-content-center justify-content-md-between py-3 px-5 mb-4 border-bottom">
      <div class="col-md-3 mb-2 mb-md-0">
        <a href="/" class="d-inline-flex link-body-emphasis text-decoration-none">
          ESP GPS
        </a>
      </div>

      <ul class="nav col-12 col-md-auto mb-2 justify-content-center mb-md-0">
        <WifiIcon :connected="true" :rssi="-30"/>
        <CellularIcon :connected="true"/>
      </ul>

      <div class="col-md-3 text-end">
        Time: 15:15
      </div>
    </header>
  </div>
  <div class="container">
    whats good
  </div>
</template>

<style scoped>

</style>
