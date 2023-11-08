<script setup>

const DEFAULT_MQTT_PORT = 1883;

</script>

<template>
  <form>
    <div class="row mb-3">
      <div class="col">
        <input type="text" class="form-control" placeholder="Username">
      </div>
      <div class="col">
        <input type="password" class="form-control" placeholder="Password">
      </div>
    </div>
    <div class="row mb-3">
      <div class="col">
        <div class="input-group">
          <span class="input-group-text">Broker settings</span>
          <input type="text" class="form-control" placeholder="Server address">
          <input type="number" class="form-control" :placeholder="DEFAULT_MQTT_PORT">
        </div>
      </div>
    </div>
    <div class="text-end">
      <button type="submit" class="btn btn-primary text-end">Save changes</button>
    </div>
  </form>
</template>

<style scoped>

</style>
