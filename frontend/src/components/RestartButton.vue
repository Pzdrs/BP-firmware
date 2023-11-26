<script setup>
import {restartEndpoint} from "../axios/endpoints";

function restart() {
  restartEndpoint.post()
      .then(() => {
        alert('Restarting...');
      })
      .catch(() => {
        alert('Failed to restart');
      });
}
</script>

<template>
  <a @click.prevent="restart" href="" class="text-danger" title="Restart">
    <Icon name="md-restartalt"/>
  </a>
</template>

<style scoped>

</style>
