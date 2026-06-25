#ifndef RTERROR_H
# define RTERROR_H
# include <stdint.h>
# include <errno.h>

// Custom error messages
# define EMSG_1 "Insufficient line in .rt file."
# define EMSG_2 "Unknown identifier in .rt file."
# define EMSG_3 "Unknown/wrong symbol in .rt file."
# define EMSG_4C "Unsupported amount of cameras (C)."
# define EMSG_4A "Unsupported amount of ambient light (A)."
# define EMSG_4O "Unsupported amount of objects (sp, cy, pl)."

/**
 * @brief Custom error code macros (range <= -2).
 * @param ERTRN A fixed option, which returns the set value.
 * @param ESET A fixed option (errset()),
 * or returned by errset() when waiting for address.
 * @param ERRNSET Returned by errset() when static is NULL.
 * @param ERRFORM Format error (in file).
 * @param ERRFEXT Wrong file extension.
 */
enum	e_custom_error
{
	ERTRN = -1,
	ESET = -2,
	ERRNSET = -3,
	ERRFORM = -4,
	ERRFEXT = -5
};

// error.c
int16_t	errset(const int64_t errnum);

// print.c
int16_t	perr(char *place, int16_t num);
int16_t	perr_msg(char *place, const int16_t errnum, char *msg);

#endif
