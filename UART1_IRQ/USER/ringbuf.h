#ifndef __RINGBUF_H__
#define __RINGBUF_H__


#include"sys.h"

/**
 * \brief      Structure that holds the state of a ring buffer.
 *
 *             This structure holds the state of a ring buffer. The
 *             actual buffer needs to be defined separately. This
 *             struct is an opaque structure with no user-visible
 *             elements.
 *
 */
struct ringbuf {
  uint8_t *data;
  uint16_t mask;
  
  /* XXX these must be 8-bit quantities to avoid race conditions. */
  uint16_t put_ptr, get_ptr;
};
extern struct ringbuf ring_rx;
extern struct ringbuf ring_tx;
/**
 * \brief      Initialize a ring buffer +
 * \param r    A pointer to a struct ringbuf to hold the state of the ring buffer
 * \param a    A pointer to an array to hold the data in the buffer
 * \param size_power_of_two The size of the ring buffer, which must be a power of two
 *
 *             This function initiates a ring buffer. The data in the
 *             buffer is stored in an external array, to which a
 *             pointer must be supplied. The size of the ring buffer
 *             must be a power of two and cannot be larger than 128
 *             bytes.
 *
 */
void    ringbuf_init(struct ringbuf *r, uint8_t *a,
		     uint16_t size_power_of_two);

/**
 * \brief      Insert a byte into the ring buffer
 * \param r    A pointer to a struct ringbuf to hold the state of the ring buffer
 * \param c    The byte to be written to the buffer
 * \return     Non-zero if there data could be written, or zero if the buffer was full.
 *
 *             This function inserts a byte into the ring buffer. It
 *             is safe to call this function from an interrupt
 *             handler.
 *
 */
int     ringbuf_put(struct ringbuf *r, uint8_t c);


/**
 * \brief      Get a byte from the ring buffer
 * \param r    A pointer to a struct ringbuf to hold the state of the ring buffer
 * \return     The data from the buffer, or -1 if the buffer was empty
 *
 *             This function removes a byte from the ring buffer. It
 *             is safe to call this function from an interrupt
 *             handler.
 *
 */
int     ringbuf_get(struct ringbuf *r);

/**
 * \brief      Get the size of a ring buffer
 * \param r    A pointer to a struct ringbuf to hold the state of the ring buffer
 * \return     The size of the buffer.
 */
int     ringbuf_size(struct ringbuf *r);

/**
 * \brief      Get the number of elements currently in the ring buffer
 * \param r    A pointer to a struct ringbuf to hold the state of the ring buffer
 * \return     The number of elements in the buffer.
 */
int     ringbuf_elements(struct ringbuf *r);

#endif /* __RINGBUF_H__ */
